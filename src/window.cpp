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

#include "window.h"

#include <cassert>

#include "game.h"
#include "logger.h"

Window::Window(unsigned width,
               unsigned height,
               const std::string &title) :
    _renderWindow(nullptr)
{
    _width = width ? width : getGame().getOptions().windowWidth;
    _height = height ? height : getGame().getOptions().windowHeight;
    _title = title.length() ? title : getGame().getOptions().programName;

    _renderWindow = getGame().getOgreRoot()->initialise(true, _title);
    Ogre::WindowEventUtilities::addWindowEventListener(_renderWindow, this);
    
    LOG_INFO << "created window " << this;
}

Window::~Window()
{
    if (_renderWindow) _renderWindow->destroy();
    
    LOG_DEBUG << "destroyed window";
}

std::string Window::toString() const
{
    std::ostringstream ss;
    ss << "Window[width = " << _width << ", height = " << _height << "]";
    return ss.str();
}

void Window::windowResized(Ogre::RenderWindow *renderWindow)
{
    assert(renderWindow == _renderWindow);
}

void Window::windowClosed(Ogre::RenderWindow *renderWindow)
{
    assert(renderWindow == _renderWindow);  
}
