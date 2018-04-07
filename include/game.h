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

#ifndef __OGRE_GAME_H__
#define __OGRE_GAME_H__

#include "defines.h"

#include <OgreFrameListener.h>
#include <OgreRoot.h>

#include "entitymanager.h"
#include "events.h"
#include "inputmanager.h"
#include "stringable.h"
#include "window.h"

class Game : public Stringable,
             public Events::Subscriber,
             public Ogre::FrameListener
{
public:
    struct Options
    {
        std::string programName;
        unsigned windowWidth, windowHeight;
        std::string logFile;
        bool suppressOgreLog;
        bool showConfigDialog;
    };

    Game(const Options &options);

    void run();
    void exit(const std::exception *e = nullptr);

    inline bool isRunning() { return _running; }
    inline void setRunning(bool b) { _running = b; }

    inline const Options &getOptions() const { return _options; }
	inline Ogre::Root *getOgreRoot() const { return _root; }
    inline Ogre::SceneManager *getOgreSceneMgr() { return _sceneMgr; }
    inline Window *getWindow() const { return _window; }
    inline InputManager *getInputMgr() const { return _inputMgr; }
    inline EntityManager *getEntityMgr() const { return _entityMgr; }
    
    void onEvent(const Events::Quit &event);
    
    std::string toString() const override { return "Game[]"; }

private:
    // Game options and internal state
    Options _options;
    bool _running;
    Window *_window;
    InputManager *_inputMgr;
    EntityManager *_entityMgr;
	
	// OGRE variables
	Ogre::Root *_root;
	Ogre::String _resourcesCfg;
	Ogre::String _pluginsCfg;
    Ogre::SceneManager *_sceneMgr;
    Ogre::Camera *_camera;

	void parseOgreResourcesConfig();
    void debugSetup();
    bool frameRenderingQueued(const Ogre::FrameEvent &e) override;
};

// Global game instance getter (in main.cpp)
Game *getGame();

#endif

