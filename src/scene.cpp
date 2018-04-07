#include "scene.h"

#include "components/camera.h"
#include "components/light.h"
#include "entity.h"

Scene::Scene()
{
    auto entity = Entity::create("Ogre");
}

Scene::~Scene()
{
}
