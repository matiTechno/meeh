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
Main::InitOptions Main::initOptions;
SDL_Window* Main::sdlWindow;
FrameInfo Main::frameInfo;
std::array<Vertex, 50000> Main::vertices;
std::array<glm::mat4, 10000> Main::matrices;
std::vector<Main::Batch> Main::batches;
bool Main::quitV = false;

Main::~Main() = default;

Main::Main()
{
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
    auto sdlInitFlags = SDL_INIT_VIDEO;
    if(initOptions.initMixer)
        sdlInitFlags |= SDL_INIT_AUDIO;

    if(SDL_Init(sdlInitFlags))
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        throw std::exception();
    }
    cleanSdlInit.set([](){SDL_Quit();});

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

    SDL_GL_SetSwapInterval(1);
}

Main::Renderer::Renderer()
{}

void Main::start(std::unique_ptr<Scene> scene)
{
    (void)scene;
    loop();
}

void Main::loop()
{
    while(!quitV)
    {
        renderer->render();
    }
}

void Main::Renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(getSdlWindow());
}

} // meeh
