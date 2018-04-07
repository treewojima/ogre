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

#ifndef __OGRE_UUID_H__
#define __OGRE_UUID_H__

#include "defines.h"

#ifdef _USE_CUSTOM_UUID
#   include <cstdint>
#else
#   include <boost/uuid/uuid.hpp>
#   include <boost/uuid/uuid_io.hpp> // for uuid ostream operator
#endif

namespace uuid
{
#ifdef _USE_CUSTOM_UUID
    typedef uint64_t uuid;
#else
    typedef boost::uuids::uuid uuid;
#endif

    void initialize();
    uuid generate();
}

#endif

