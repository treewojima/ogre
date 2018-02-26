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

#ifndef __OGRE_WINDOW_H__
#define __OGRE_WINDOW_H__

#include "defines.h"

#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>

#include "stringable.h"

//#include "exceptions.hpp"

class Window : public Stringable,
               public Ogre::WindowEventListener
{
    friend class InputManager;
    
public:
    Window(unsigned width = 0,
           unsigned height = 0, 
           const std::string &title = "");
    ~Window();
    
    bool isClosed() const { return _renderWindow->isClosed(); }
        
    std::string toString() const override;
    
private:
    unsigned _width;
    unsigned _height;
    std::string _title;
    Ogre::RenderWindow *_renderWindow;

    void windowResized(Ogre::RenderWindow *renderWindow) override;
    void windowClosed(Ogre::RenderWindow *renderWindow) override;
};

/*namespace Exceptions
{
    class WindowException : public Base
    {
    public:
        using Base::Base;
    };
}*/

#endif
