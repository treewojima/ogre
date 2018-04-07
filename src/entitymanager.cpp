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

#include "entitymanager.h"
#include <algorithm>

EntityManager::EntityManager()
{
    //Events::Dispatcher::subscribe<Events::EntityCreated>(*this);
    Events::Dispatcher::subscribe<Events::ComponentCreated>(*this);
}

EntityManager::~EntityManager()
{
    // TODO: update this as necessary for additional cleanup procedures

    //for (auto &pair : _map)
    //{
    //}

    Events::Dispatcher::unsubscribe(*this);
    _map.clear();
}

Entity::UUID EntityManager::createEntity(const std::string &debugName)
{
    auto uuid = uuid::generate();

    if (_map.count(uuid))
    {
        throw Exceptions::EntityExists(uuid);
    }
    if (_debugNameMap.count(uuid))
    {
        LOG_WARNING << "entity with UUID " << uuid << " already exists in "
                    << "_debugNameMap, but not in _map";
    }

    _map[uuid] = ComponentMap();
    _debugNameMap[uuid] = debugName;

    Events::Dispatcher::raise<Events::EntityCreated>(uuid);

#ifdef _DEBUG_ENTITIES
    LOG_DEBUG << "created entity " << Entity(uuid).toString();
#endif

    return uuid;
}

void EntityManager::destroyEntity(UUID uuid)
{
    // NOTE: there should be some more deconstruction here

    auto iter = _map.find(uuid);
    if (iter == _map.end())
    {
        throw Exceptions::NoSuchEntity(uuid);
    }
    _map.erase(iter);

    auto iter2 = _debugNameMap.find(uuid);
    if (iter2 == _debugNameMap.end())
    {
        LOG_WARNING << "entity with UUID " << uuid << " existed in _map, but "
                    << "not in _debugNameMap";
    }
    else
    {
        _debugNameMap.erase(iter2);
    }

    Events::Dispatcher::raise<Events::EntityDestroyed>(uuid);
}

/*void EntityManager::onEvent(const Events::EntityCreated &event)
{
    EntityComponentsPair pair;
    pair.first = event.entity;

    _map[event.entity->getUUID()] = pair;
}*/

void EntityManager::onEvent(const Events::ComponentCreated &event)
{
    auto component = event.component;

    try
    {
        auto &componentMap = _map.at(component->getParent());
        if (componentMap.count(typeid(*component)))
        {
            throw Exceptions::ComponentExists(component);
        }
        else
        {
            componentMap[typeid(*component)] = component;
        }
    }
    catch (std::out_of_range)
    {
        throw Exceptions::NoSuchEntity(component->getParent());
    }
}

const std::string &EntityManager::getDebugName(UUID uuid) const
{
    try
    {
        return _debugNameMap.at(uuid);
    }
    catch (std::out_of_range)
    {
        throw Exceptions::NoSuchEntity(uuid);
    }
}

std::string EntityManager::toString() const
{
    std::ostringstream ss;
    ss << "EntityManager[entityCount = " << _map.size() << "]";
    return ss.str();
}
