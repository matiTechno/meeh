#include <meeh/Scene.hpp>
#include <meeh/Main.hpp>

namespace meeh
{

Scene::Scene()
{assert(Main::isInitialized());}

void Scene::beginFrame() {}
void Scene::processInput() {}
void Scene::whenNotTop() {}
void Scene::update() {}
void Scene::render() {}

} // meeh
