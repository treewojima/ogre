/* game
 * Copyright (C) 2014-2016 Scott Bishop <treewojima@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __OGRE_ENTITY_H__
#define __OGRE_ENTITY_H__

#include "defines.h"

#include <boost/core/demangle.hpp>
#include <memory>
#include <sstream>

#include "events.h"
#include "pool.h"
#include "stringable.h"
#include "uuid.h"

static constexpr unsigned COMPONENT_POOL_SIZE = 100;

#ifdef _DEBUG
#   define _DEBUG_ENTITIES
#endif

namespace Components { class Component; }

// Conceptually, an Entity should be nothing more than an identifier. This
// class is a light convenience wrapper around functionality that EntityManager
// provides
class Entity final : public Stringable
{
    friend class EntityManager;

public:
    typedef uuid::uuid UUID;

    // Call this to create and register the Entity with the EntityManager
    static UUID create(const std::string &debugName);

    // Call this constructor to do the above, as well as provide a light
    // class instance to allow for chaining Component calls and using
    // toString() (and nothing more)
    Entity(const std::string &debugName);

    // Call this constructor to create the Entity instance with an existing
    // UUID, without registering it as a new entity
    Entity(UUID uuid) : _uuid(uuid) {}

    inline UUID getUUID() const { return _uuid; }

    const std::string &getDebugName() const;

    // Helper method for chaining component creation calls
    template <class C, class... Args>
    Entity &component(Args&& ... args)
    {
        static_assert(std::is_base_of<Components::Component, C>::value,
                      "Can only add components of a type derived from class Components::Component");

        C::create(_uuid, std::forward<Args>(args)...);

        return *this;
    }

    std::string toString() const;

private:
    UUID _uuid;
};

// The component base class is in this file to prevent cyclic preprocessor includes
namespace Components
{
    class Component : public Stringable
    {
    protected:
        Component(const Entity::UUID &parentUUID, const std::string &debugName = "Component");

    public:
        virtual ~Component();

        inline std::string getDebugName() const { return _debugName; }
        inline Entity::UUID getParent() const { return _parentUUID; }

        std::string toString() const override;

    protected:
        //inline void setDebugName(const std::string &name) { _debugName = name; }
        inline void setDebugName(std::string &&name) { _debugName = std::move(name); }

    private:
        Entity::UUID _parentUUID;
        std::string _debugName;
    };
}

// Entity and component events
namespace Events
{
    class EntityCreated : public Events::Event
    {
    public:
        Entity::UUID uuid;

        EntityCreated(Entity::UUID uuid_) :
            Events::Event(),
            uuid(uuid_) {}

        std::string toString() const
        {
            std::ostringstream ss;
            ss << "Events::EntityCreated[uuid = " << uuid << "]";
            return ss.str();
        }
    };

    class EntityDestroyed : public Events::Event
    {
    public:
        Entity::UUID uuid;

        EntityDestroyed(Entity::UUID uuid_) :
            Events::Event(),
            uuid(uuid_) {}


        std::string toString() const
        {
            std::ostringstream ss;
            ss << "Events::EntityDestroyed[uuid = " << uuid << "]";
            return ss.str();
        }
    };

    template <class T>
    class SpecificComponentCreated : public Events::Event
    {
    public:
        T *component;

        SpecificComponentCreated(T *component_) :
            Events::Event(),
            component(component_) {}

        std::string toString() const
        {
            std::ostringstream ss;
            ss << "Events::SpecificComponentCreated<"
               << boost::core::demangle(typeid(T).name()) << ">"
               << "[component = " << component << "]";
            return ss.str();
        }
    };

    typedef SpecificComponentCreated<Components::Component> ComponentCreated;
}

#endif
