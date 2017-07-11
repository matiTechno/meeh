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
    bool isTop() const {return mIsTop;}

private:
    friend class Main;
    bool mIsTop = true;
};

class Scene
{
public:
    Scene();

    virtual ~Scene() = default;

    virtual void beginFrame();

    // only if isTop == true
    virtual void processInput();

    virtual void update();

    virtual void render();

protected:
    void popScenes(int num);

    void pushScene(std::unique_ptr<Scene> scene);

    SceneProperties prop;
};

} // meeh
