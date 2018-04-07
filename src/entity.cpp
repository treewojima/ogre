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

#include "entity.h"

#include <sstream>

#include "entitymanager.h"
#include "events.h"
#include "game.h"
#include "logger.h"

Entity::UUID Entity::create(const std::string &debugName)
{
    return getGame()->getEntityMgr()->createEntity(debugName);
}

Entity::Entity(const std::string &debugName)
{
    _uuid = create(debugName);
}

const std::string &Entity::getDebugName() const
{
    return getGame()->getEntityMgr()->getDebugName(_uuid);
}

std::string Entity::toString() const
{
    std::ostringstream ss;
    ss << "Entity[uuid = " << _uuid
       << ", debugName = \"" << getDebugName() << "\"]";
    return ss.str();
}

Components::Component::Component(const Entity::UUID &parentUUID,
                                 const std::string &debugName) :
    _parentUUID(parentUUID)
{
    setDebugName(Entity(parentUUID).getDebugName() + debugName);
}

Components::Component::~Component()
{
#ifdef _DEBUG_COMPONENTS
    LOG_DEBUG << "destroyed component \"" << getDebugName() << "\"";
#endif
}

std::string Components::Component::toString() const
{
    std::ostringstream ss;
    ss << "Components::Component[parent = " << getParent() << ", "
       << "debugName = \"" << getDebugName() << "\"]";
    return ss.str();
}
