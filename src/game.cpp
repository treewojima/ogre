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

#include "game.h"

#include <OgreConfigFile.h>
#include <OgreException.h>
#include <sstream>

#include "logger.h"

Game::Game(const Options &options) :
    _options(options),
    _running(false),
    _window(nullptr),
    _inputMgr(nullptr),
	_root(nullptr),
	_resourcesCfg(Ogre::BLANKSTRING),
	_pluginsCfg(Ogre::BLANKSTRING)
{
    Logger::init(options.logFile);
}

void Game::run()
{
	// Create and configure the OGRE root object 
	_resourcesCfg = "resources.cfg";
	_pluginsCfg = "plugins.cfg";
	_root = new Ogre::Root(_pluginsCfg);
	parseOgreConfig();

    _window = new Window();
    _inputMgr = new InputManager();
    
    delete _inputMgr;
    delete _window;
	delete _root;
}

void Game::exit(const std::exception *e)
{
    std::ostringstream ss;
    ss << "EXIT: ";

    if (e)
    {
        ss << e->what();
    }
    else
    {
        ss << "exit() called";
    }

    LOG_ERROR << ss.str();
    std::exit(1);
}

void Game::parseOgreConfig()
{
	Ogre::ConfigFile cf;
	cf.load(_resourcesCfg);

	Ogre::String name, type;
	auto si = cf.getSectionIterator();
	auto settings = si.getNext();
	for (auto i = settings->begin(); i != settings->end(); ++i)
	{
		type = i->first;
		name = i->second;
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, type);
	}
    
    if (!_root->restoreConfig()) _root->showConfigDialog();
}

bool Game::frameRenderingQueued(const Ogre::FrameEvent &e)
{
}
