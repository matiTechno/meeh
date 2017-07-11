#pragma once

#include <vector>
#include <memory>
#include <SDL2/SDL_events.h>
#include <glm/vec2.hpp>
struct SDL_Window;
#include <array>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <mutex>

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
    SDL_Window* sdlWindow;
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

    struct
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
    }
    init;

    void start(std::unique_ptr<Scene> scene);

    static void addInstance(Vertex* vertices, int count, const glm::mat4& matrix = glm::mat4(1.f));

    static const FrameInfo& getFrameInfo() {return frameInfo;}

private:
    std::vector<std::unique_ptr<Scene>> scenes;
    static FrameInfo frameInfo;
    static std::array<Vertex, 10000> vertices;
    static std::array<glm::mat4, 10000> matrices;
    struct Batch
    {
        int start;
        int num;
        int numPerInstance;
    };
    static std::vector<Batch> batches;
    static std::mutex mutex;

    void loop();
    static void renderFunction();

    class Renderer
    {
    public:

    private:
    };
};

} // meeh
