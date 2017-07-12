set(SOURCE
    Main.cpp
    Scene.cpp
    glad.c
    GlObjects.cpp
    Texture.cpp
    Shader.cpp
    Sound.cpp
    Font.cpp
    stb_image.h

    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_impl_sdl_gl3.cpp
    imgui/imgui_impl_sdl_gl3.h
    imgui/imgui_internal.h
    imgui/stb_rect_pack.h
    imgui/stb_textedit.h
    imgui/stb_truetype.h
    )

add_definitions(-DIMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION)
add_library(meeh ${SOURCE})
target_link_libraries(meeh -lSDL2 -lSDL2_mixer)
