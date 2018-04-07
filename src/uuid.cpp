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

#include "uuid.h"

#ifdef _USE_CUSTOM_UUID
#   include <cstdlib>
#   include <ctime>
#else
#   include <boost/uuid/random_generator.hpp>
#endif

namespace
{
    bool _initialized = false;

#ifdef _USE_CUSTOM_UUID
    uuid::uuid _state[2];
#else
    boost::uuids::random_generator _generator;
#endif
}

void uuid::initialize()
{
#ifdef _USE_CUSTOM_UUID
    using namespace std;

    srand(time(nullptr));
    _state[0] = rand();
    _state[1] = rand();
#endif

    _initialized = true;
}

uuid::uuid uuid::generate()
{
    if (!_initialized) initialize();

#ifdef _USE_CUSTOM_UUID
    // This uses the xorshift128+ random number method, it's basically a
    // random number generator
    uuid x = _state[0];
    uuid const y = _state[1];
    _state[0] = y;
    x ^= x << 23;
    _state[1] = x ^ y ^ (x >> 17) ^ (y >> 26);
    return _state[1] + y;
#else
    return _generator();
#endif
}
