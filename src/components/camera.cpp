#include "components/camera.h"
#include <sstream>
#include "game.h"

namespace Components {

Camera *Camera::create(const Entity::UUID &parent,
                       const std::string &debugName)
{
    auto ptr = new Camera(parent, debugName);
    Events::Dispatcher::raise<Events::ComponentCreated>(ptr);
    Events::Dispatcher::raise<Events::CameraComponentCreated>(ptr);
    return ptr;
}

Camera::Camera(const Entity::UUID &parent,
               const std::string &debugName) :
    Component(parent, debugName),
    ogreSceneNode(nullptr),
    ogreCamera(nullptr)
{
    auto sceneMgr = getGame()->getOgreSceneMgr();
    
    ogreCamera = sceneMgr->createCamera("MainCamera");
    ogreCamera->setNearClipDistance(5);
    ogreSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    ogreSceneNode->attachObject(ogreCamera);
    ogreSceneNode->setPosition(0, 0, 80);
    ogreSceneNode->lookAt(Ogre::Vector3(0, 0, -300), Ogre::SceneNode::TS_WORLD);
}

std::string Camera::toString() const
{
    std::ostringstream ss;
    ss << "Components::Camera";
    return ss.str();
}

} // namespace Components