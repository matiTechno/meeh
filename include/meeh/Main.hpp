#pragma once

#include <vector>
#include <memory>
#include <SDL2/SDL_events.h>
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
struct SDL_Window;

namespace meeh
{

class Scene;

struct FrameInfo
{
    std::vector<SDL_Event> events;
    float frametime;
    float avFrametime;
    glm::ivec2 fbSize;
    SDL_Window* sdlWindow;
};

class Main
{
public:
    Main();

private:
    std::vector<std::unique_ptr<Scene>> scenes;
    FrameInfo frameInfo;
};

} // meeh
