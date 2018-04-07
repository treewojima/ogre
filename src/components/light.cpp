#include "components/light.h"
#include <sstream>
#include "game.h"

namespace Components {

Light *Light::create(const Entity::UUID &parent,
                     const std::string &debugName)
{
    auto ptr = new Light(parent, debugName);
    Events::Dispatcher::raise<Events::ComponentCreated>(ptr);
    Events::Dispatcher::raise<Events::LightComponentCreated>(ptr);
    return ptr;
}

Light::Light(const Entity::UUID &parent,
             const std::string &debugName) :
    Component(parent, debugName),
    ogreSceneNode(nullptr),
    ogreLight(nullptr)
{
    auto sceneMgr = getGame()->getOgreSceneMgr();
    
    ogreLight = sceneMgr->createLight("MainLight");
    ogreSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    ogreSceneNode->attachObject(ogreLight);
    ogreSceneNode->setPosition(20, 80, 50);
}

std::string Light::toString() const
{
    std::ostringstream ss;
    ss << "Components::Light";
    return ss.str();
}

} // namespace Components