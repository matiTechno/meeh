#include <meeh/Main.hpp>
#include <meeh/Scene.hpp>
#include <meeh/imgui.h>
#include "imgui/imgui_impl_sdl_gl3.h"
#include <meeh/glad.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_mixer.h>

namespace meeh
{

Main* Main::mainPtr = nullptr;
InitOptions Main::initOptions;
SDL_Window* Main::sdlWindow;
FrameInfo Main::frameInfo;
std::vector<Vertex> Main::vertices;
std::vector<glm::mat4> Main::matrices;
std::vector<Main::Batch> Main::batches;
bool Main::quitV = false;
int Main::numToPop = 0;

Main::~Main() = default;

Main::Main(const InitOptions& initOptions)
{
    this->initOptions = initOptions;
    mainPtr = this;
    {
        SDL_version verCompiled, verLinked;
        SDL_VERSION(&verCompiled);
        SDL_GetVersion(&verLinked);
        printf("compiled against SDL       %d.%d.%d\n"
               "linked   against SDL       %d.%d.%d\n",
               verCompiled.major, verCompiled.minor, verCompiled.patch,
               verLinked.major, verLinked.minor, verLinked.patch);
    }
    {
        SDL_version verCompiled;
        SDL_MIXER_VERSION(&verCompiled);
        const SDL_version* verLinked = Mix_Linked_Version();
        printf("compiled against SDL_mixer %d.%d.%d\n"
               "linked   against SDL_mixer %d.%d.%d\n",
               verCompiled.major, verCompiled.minor, verCompiled.patch,
               verLinked->major, verLinked->minor, verLinked->patch);
    }
    fflush(stdout);

    auto sdlInitFlags = SDL_INIT_VIDEO;
    if(initOptions.initAudio)
        sdlInitFlags |= SDL_INIT_AUDIO;

    if(SDL_Init(sdlInitFlags))
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        throw std::exception();
    }
    cleanSdlInit.set([](){SDL_Quit();});

    if(initOptions.initAudio)
        initAudio();

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, initOptions.glMajor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, initOptions.glMinor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    sdlWindow = SDL_CreateWindow(initOptions.title, initOptions.posX, initOptions.posY,
                                 initOptions.sizeX, initOptions.sizeY,
                                 SDL_WINDOW_OPENGL | initOptions.sdlWindowFlags);
    if(!sdlWindow)
    {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        throw std::exception();
    }
    cleanSdlWindow.set([](){SDL_DestroyWindow(sdlWindow);});

    auto context = SDL_GL_CreateContext(sdlWindow);
    if(!context)
    {
        printf("SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        throw std::exception();
    }
    cleanSdlContext.set([context](){SDL_GL_DeleteContext(context);});

    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
    {
        printf("gladLoadGLLoader failed\n");
        throw std::exception();
    }

    printf("OpenGL version: %s\n"
           "vendor:         %s\n"
           "renderer:       %s\n",
           glGetString(GL_VERSION), glGetString(GL_VENDOR), glGetString(GL_RENDERER));
    fflush(stdout);

    SDL_GL_SetSwapInterval(1);
    SDL_GL_GetDrawableSize(sdlWindow, &frameInfo.fbSize.x, &frameInfo.fbSize.y);
    glEnable(GL_BLEND);
    renderer = std::make_unique<Renderer>();
    if(initOptions.initImgui)
    {
        ImGui_ImplSdlGL3_Init(sdlWindow);
        cleanImgui.set([](){ImGui_ImplSdlGL3_Shutdown();});
    }
    vertices.reserve(initOptions.reservedVertices);
    matrices.reserve(initOptions.reservedInstances);

    Batch batch;
    batch.start = 0;
    batch.size = 0;
    batch.startMatrices = 0;
    batch.primitive = GL_TRIANGLES;
    batch.texture = nullptr;
    batch.sampler = nullptr;
    batch.srcAlpha = GL_SRC_ALPHA;
    batch.dstAlpha = GL_ONE_MINUS_SRC_ALPHA;
    batch.isFont = false;

    batches.push_back(batch);
}

void Main::start(std::unique_ptr<Scene> scene)
{
    assert(wasStarted == false);
    wasStarted = true;
    assert(scene);
    scenes.push_back(std::move(scene));
    loop();
}

void Main::loop()
{
    int time = SDL_GetTicks();

    while(!quitV || scenes.empty())
    {
        addedScenes.clear();
        numToPop = 0;
        frameInfo.events.clear();

        int newTime = SDL_GetTicks();
        frameInfo.frametime = (newTime - time) / 1000.f;
        newTime = time;

        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(initOptions.initImgui)
                ImGui_ImplSdlGL3_ProcessEvent(&e);

            if(e.type == SDL_QUIT && initOptions.handleQuit)
                quitV = true;
            frameInfo.events.push_back(e);
        }
        if(initOptions.initImgui)
        {
            ImGui_ImplSdlGL3_NewFrame(sdlWindow);
            frameInfo.imguiWantsInput = ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantCaptureMouse;
        }
        SDL_GL_GetDrawableSize(sdlWindow, &frameInfo.fbSize.x, &frameInfo.fbSize.y);
        frameInfo.fbAspect = frameInfo.fbSize.x / frameInfo.fbSize.y;

        for(auto& scene: scenes)
            scene->beginFrame();
        for(auto& scene: scenes)
        {
            if(scene->prop.isTop_)
                scene->whenNotTop();
            else
                scene->processInput();
        }
        for(auto& scene: scenes)
        {
            if(scene->prop.updateWhenNotTop || scene->prop.isTop_)
                scene->update();
        }
        scenesToRender.clear();
        for(auto it = scenes.rbegin(); it != scenes.rend(); ++it)
        {
            auto& scene = **it;
            assert(scene.prop.size.x >= 0 && scene.prop.size.y >= 0);
            scenesToRender.push_back(&scene);
            if(scene.prop.isOpaque)
                break;
        }
        for(auto it = scenesToRender.rbegin(); it != scenesToRender.rend(); ++it)
        {
            auto& scene = **it;
            setBatchViewport(scene);
            scene.render();
        }

        glClear(GL_COLOR_BUFFER_BIT);
        renderer->render();
        if(initOptions.initImgui)
            ImGui::Render();
        SDL_GL_SwapWindow(getSdlWindow());

        for(int i = 0; i < numToPop; ++i)
            scenes.pop_back();
        for(auto& newScene: addedScenes)
            scenes.push_back(std::move(newScene));

        for(auto& scene: scenes)
            scene->prop.isTop_ = false;
        if(scenes.size())
            scenes.back()->prop.isTop_ = true;
    }
}

void Main::popCurrentScenes(unsigned num)
{
    assert(numToPop == 0);
    assert(num <= mainPtr->scenes.size());
    numToPop = num;
}

void Main::pushScene(std::unique_ptr<Scene> scene)
{
    assert(scene);
    mainPtr->addedScenes.push_back(std::move(scene));
}

void Main::initAudio()
{
    if(initOptions.mixerFlags)
    {
        int initted = Mix_Init(initOptions.mixerFlags);
        if((initted & initOptions.mixerFlags) != initOptions.mixerFlags)
            printf("Mix_Init failed\n"
                   "required flags: %d\n"
                   "returned flags: %d\n"
                   "%s\n", initOptions.mixerFlags, initted, Mix_GetError());

        cleanSdlMixerInit.set([](){Mix_Quit();});
    }
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
        printf("Mix_OpenAudio failed: %s\n", Mix_GetError());
        throw std::exception();
    }
    cleanSdlMixerAudio.set([](){Mix_CloseAudio();});
    Mix_AllocateChannels(initOptions.reservedMixerChannels);
}

void Main::addBatch()
{
    auto& prevBatch = batches.back();
    if(prevBatch.size)
    {
        batches.push_back(prevBatch);
        auto& batch = batches.back();
        batch.start = prevBatch.start + prevBatch.size;
        batch.size = 0;
        batch.startMatrices = prevBatch.start + prevBatch.size / prevBatch.numPerInstance;
    }
}

void Main::setBatchViewport(const Scene& scene)
{
    glm::ivec4 viewport(scene.prop.pos.x, frameInfo.fbSize.y - (scene.prop.pos.y + scene.prop.size.y),
                        scene.prop.size.x, scene.prop.size.y);
    if(batches.back().viewport != viewport)
        addBatch();
    batches.back().viewport = viewport;
}

void Main::addInstance(Vertex* vertices, int count, const glm::mat4& model)
{
    if(count != batches.back().numPerInstance)
        addBatch();
    auto& batch = batches.back();
    batch.numPerInstance = count;
    for(int i = 0; i < count; ++i)
        Main::vertices[batch.start + batch.size + i] = vertices[i];
    matrices[batch.startMatrices + batch.size / count] = model;
    batch.size += count;
}

void Main::flushGl()
{
    mainPtr->renderer->render();
}

void Main::setGlPrimitive(GLenum primitive)
{
    if(batches.back().primitive != primitive)
        addBatch();
    batches.back().primitive = primitive;
}

void Main::setTexture(Texture* texture)
{
    if(batches.back().texture != texture)
        addBatch();
    batches.back().texture = texture;
}

void Main::setSampler(GlSampler* sampler)
{
    if(batches.back().sampler != sampler)
        addBatch();
    batches.back().sampler = sampler;
}

void Main::setBlendFunc(GLenum srcAlpha, GLenum dstAlpha)
{
    if(batches.back().srcAlpha != srcAlpha || batches.back().dstAlpha != dstAlpha)
        addBatch();
    batches.back().srcAlpha = srcAlpha;
    batches.back().dstAlpha = dstAlpha;
}

void Main::setProjection(const glm::mat4& matrix)
{
    addBatch();
    batches.back().projection = matrix;
}

void Main::setFontMode(bool on)
{
    if(batches.back().isFont != on)
        addBatch();
    batches.back().isFont = on;
}

Main::Renderer::Renderer()
{}

void Main::Renderer::render()
{
    for(auto& batch: batches)
    {
        (void)batch;
    }
    auto& batch = batches.front();
    batch = batches.back();
    batch.start = 0;
    batch.size = 0;
    batch.startMatrices = 0;
    batches.erase(batches.begin() + 1, batches.end());
}

} // meeh
