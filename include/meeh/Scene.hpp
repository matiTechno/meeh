#pragma once

#include <glm/vec2.hpp>
#include <memory>

namespace meeh
{

struct SceneProperties
{
    glm::ivec2 pos{0, 0};
    glm::ivec2 size;
    bool isOpaque = true;
    bool updateWhenNotTop = false;
    bool autoFitToFb = true;
    bool isTop = true;
};

class Scene
{
public:
    Scene();

    virtual ~Scene() = default;

    virtual void beginFrame();

    // only if isTop == true
    virtual void processInput();
    // else
    // useful for unsetting things when new scene
    // is pushed onto the stack
    virtual void whenNotTop();

    virtual void update();

    virtual void render();

protected:
    SceneProperties prop;
};

} // meeh
