#ifndef __OGRE_COMPONENTS_CAMERA_H__
#define __OGRE_COMPONENTS_CAMERA_H__

#include "defines.h"

#include <OgreCamera.h>
#include <OgreSceneNode.h>

#include "entity.h"

namespace Components
{
    class Camera : public Component
    {
    public:
        static Camera *create(const Entity::UUID &parent,
                              const std::string &debugName = "Camera");
                                     
    private:
        Camera(const Entity::UUID &parent, const std::string &debugName);
        
    public:        
        Ogre::SceneNode *ogreSceneNode;
        Ogre::Camera *ogreCamera;
        
        std::string toString() const override;
    };
}

namespace Events
{
    typedef SpecificComponentCreated<Components::Camera> CameraComponentCreated;
}

#endif
