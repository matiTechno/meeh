#pragma once

#include <vector>
#include <memory>
#include <SDL2/SDL_events.h>
#include <glm/vec2.hpp>
struct SDL_Window;
#include <array>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
class Texture;
#include "GlObjects.hpp"
#include "Shader.hpp"

namespace meeh
{

class Scene;

struct FrameInfo
{
    std::vector<SDL_Event> events;
    float frametime;
    float avFrametime;
    glm::ivec2 fbSize;
    float aspect;
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
    Main();
    ~Main();

    struct InitOptions
    {
        int sizeX = 640;
        int sizeY = 480;
        int posX = SDL_WINDOWPOS_CENTERED;
        int posY = SDL_WINDOWPOS_CENTERED;
        int glMajor = 3;
        int glMinor = 3;
        bool initImgui = false;
        bool initMixer = false;
        int sdlWindowFlags = 0;
        int mixerFlags = 0;
        bool handleQuit = true;
    };

    static InitOptions& getInitOptions() {return initOptions;}

    void start(std::unique_ptr<Scene> scene);

    static void addInstance(Vertex* vertices, int count, const glm::mat4& model = glm::mat4(1.f));

    static void flushGl();

    static const FrameInfo& getFrameInfo() {return frameInfo;}

    // executed at the end of frame
    static void popScenes(int num);
    static void pushScene(std::unique_ptr<Scene> scene);

    static SDL_Window* getSdlWindow() {return sdlWindow;}

private:
    static Main* mainPtr;
    static InitOptions initOptions;
    static SDL_Window* sdlWindow;
    std::vector<std::unique_ptr<Scene>> scenes;
    std::vector<std::unique_ptr<Scene>> addedScenes;
    static FrameInfo frameInfo;
    static std::array<Vertex, 10000> vertices;
    static std::array<glm::mat4, 10000> matrices;
    struct Batch
    {
        int start;
        int size;
        int numPerInstance;
        glm::mat4 projection;
        GLenum srcAlpha, dstAlpha;
        GLenum primitive;
        Texture* texture;
        GlSampler* sampler;
        bool isFont;
        bool flippedXCoords;
        bool projYUp;
    };
    static std::vector<Batch> batches;

    void loop();
    // static because it might be moved to
    // another thread in future
    static void render();

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
    }
    renderer;
};

} // meeh
