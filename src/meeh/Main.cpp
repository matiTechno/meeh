#include <meeh/Main.hpp>
#include <meeh/Scene.hpp>
#include <meeh/imgui.h>
#include "imgui/imgui_impl_sdl_gl3.h"
#include <meeh/glad.h>

namespace meeh
{

Main::~Main() = default;

Main::Main()
{
    ImGui_ImplSdlGL3_Init(nullptr);
}

Main::Renderer::Renderer()
{}

void Main::start(std::unique_ptr<Scene> scene)
{
    (void)scene;
}

void Main::loop()
{}

void Main::render()
{}

void Main::Renderer::render()
{
    glClearColor(0.f, 0.5f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(getSdlWindow());
}

} // meeh
