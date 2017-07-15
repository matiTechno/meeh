#pragma once

#include <vector>
#include <memory>
#include <SDL2/SDL_events.h>
#include <glm/vec2.hpp>
struct SDL_Window;
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
class Texture;
#include "GlObjects.hpp"
#include "Shader.hpp"
#include "Raii.hpp"

namespace meeh
{

class Scene;

struct InitOptions
{
    int sizeX = 640;
    int sizeY = 480;
    int posX = SDL_WINDOWPOS_CENTERED;
    int posY = SDL_WINDOWPOS_CENTERED;
    const char* title = "default";
    int glMajor = 3;
    int glMinor = 3;
    bool initImgui = false;
    bool initAudio = false;
    unsigned int sdlWindowFlags = 0;
    int mixerFlags = 0;
    bool handleQuit = true;
    int reservedInstances = 10000;
    int reservedVertices = 40000;
    int reservedMixerChannels = 8;
};

struct FrameInfo
{
    std::vector<SDL_Event> events;
    float frametime;
    glm::ivec2 fbSize;
    float fbAspect;
    bool imguiWantsInput;
};

struct Vertex
{
    glm::vec2 pos;
    glm::ivec2 texCoord;
    glm::vec4 color;
};

class Main
{
public:
    Main(const InitOptions& initOptoins = InitOptions());
    ~Main();

    void start(std::unique_ptr<Scene> scene);

    static const FrameInfo& getFrameInfo() {return frameInfo;}

    // executed at the end of frame
    // if(num > 0) no more popCurrentScenes() calls can be made this frame
    static void popCurrentScenes(unsigned num);
    static void pushScene(std::unique_ptr<Scene> scene);

    static SDL_Window* getSdlWindow() {return sdlWindow;}

    static void quit() {quitV = true;}

    static bool isInitialized() {return mainPtr ? true : false;}

    // render functions
    static void addInstance(Vertex* vertices, int count, const glm::mat4& model = glm::mat4(1.f));
    static void flushGl();
    static void setGlPrimitive(GLenum primitive);
    // pass nullptr to disable texture sampling
    static void setTexture(Texture* texture);
    // pass nullptr to use default texture sampling
    static void setSampler(GlSampler* sampler);
    static void setBlendFunc(GLenum srcAlpha, GLenum dstAlpha);
    // always creates new batch
    static void setProjection(const glm::mat4& matrix);
    static void setFontMode(bool on);

private:
    Raii cleanSdlInit;
    Raii cleanSdlWindow;
    Raii cleanSdlContext;
    Raii cleanSdlMixerInit;
    Raii cleanSdlMixerAudio;
    Raii cleanImgui;
    static Main* mainPtr;
    static bool quitV;
    static InitOptions initOptions;
    static SDL_Window* sdlWindow;
    std::vector<std::unique_ptr<Scene>> scenes;
    std::vector<Scene*> scenesToRender;
    std::vector<std::unique_ptr<Scene>> addedScenes;
    static int numToPop;
    static FrameInfo frameInfo;
    static std::vector<Vertex> vertices;
    static std::vector<glm::mat4> matrices;
    bool wasStarted = false;

    struct Batch
    {
        int start;
        int size;
        int startMatrices;
        int numPerInstance;
        glm::ivec4 viewport;
        GLenum primitive;
        Texture* texture;
        GlSampler* sampler;
        GLenum srcAlpha, dstAlpha;
        glm::mat4 projection;
        bool isFont;
    };
    static std::vector<Batch> batches;

    class Renderer
    {
    public:
        Renderer();
        void render();

    private:
        Shader shader;
        GlVAO vao;
        GlBO vboVert;
        GlBO vboInstance;
    };
    std::unique_ptr<Renderer> renderer;

    void initAudio();
    void loop();
    void setBatchViewport(const Scene& scene);
    static void addBatch();
};

} // meeh
