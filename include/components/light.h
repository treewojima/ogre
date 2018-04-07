#ifndef __OGRE_COMPONENTS_LIGHT_H__
#define __OGRE_COMPONENTS_LIGHT_H__

#include "defines.h"

#include <OgreLight.h>
#include <OgreSceneNode.h>

#include "entity.h"

namespace Components
{
    class Light : public Component
    {
    public:
        static Light *create(const Entity::UUID &parent,
                             const std::string &debugName = "Light");
                                     
    private:
        Light(const Entity::UUID &parent, const std::string &debugName);
        
    public:        
        Ogre::SceneNode *ogreSceneNode;
        Ogre::Light *ogreLight;
        
        std::string toString() const override;
    };
}

namespace Events
{
    typedef SpecificComponentCreated<Components::Light> LightComponentCreated;
}

#endif
