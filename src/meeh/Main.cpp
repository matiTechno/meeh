#include <meeh/Main.hpp>
#include <meeh/Scene.hpp>

#include <meeh/imgui.h>
#include "imgui/imgui_impl_sdl_gl3.h"

namespace meeh
{

Main::~Main() = default;

Main::Main()
{
    ImGui_ImplSdlGL3_Init(nullptr);
}

Main::Renderer::Renderer()
{}

} // meeh
