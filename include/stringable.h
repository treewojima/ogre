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

#ifndef __OGRE_STRINGABLE_H__
#define __OGRE_STRINGABLE_H__

#include "defines.h"

#ifdef _DEBUG
//#   define DEBUG_USE_SMART_PTRS
#endif

#include <string>

#if DEBUG_USE_SMART_PTRS
#   include <memory>
#   include <type_traits>
#endif

// Interface that guarantees a toString() method
class Stringable
{
public:
    virtual ~Stringable() {}
    virtual std::string toString() const = 0;
};

// Helper ostream operators
inline std::ostream &operator<<(std::ostream &os, const Stringable &obj)
{
    os << obj.toString();
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const Stringable *obj)
{
    os << (obj ? obj->toString() : "<null>");
    return os;
}

#if DEBUG_USE_SMART_PTRS
template <class T>
inline std::ostream &operator<<(std::ostream &os, const std::shared_ptr<T> &obj)
{
    static_assert(std::is_base_of<Stringable, T>::value,
                  "Class T is not subclass of type Stringable");

    os << (obj ? obj->toString() : "<null>");
    return os;
}

template <class T>
inline std::ostream &operator<<(std::ostream &os, const std::unique_ptr<T> &obj)
{
    static_assert(std::is_base_of<Stringable, T>::value,
                  "Class T is not subclass of type Stringable");

    os << (obj ? obj->toString() : "<null>");
    return os;
}
#endif

#endif
