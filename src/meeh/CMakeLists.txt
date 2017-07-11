set(SOURCE
    Main.cpp
    Scene.cpp
    )

add_library(meeh ${SOURCE})
target_link_libraries(meeh -lSDL2 -lSDL2_mixer)
