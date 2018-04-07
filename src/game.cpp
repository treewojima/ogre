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

#include <boost/filesystem.hpp>
#include <OgreCamera.h>
#include <OgreConfigFile.h>
#include <OgreEntity.h>
#include <OgreException.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreTextureManager.h>
#include <OgreViewport.h>
#include <iostream>
#include <sstream>

#include "logger.h"

namespace fs = boost::filesystem;

Game::Game(const Options &options) :
    _options(options),
    _running(false),
    _window(nullptr),
    _inputMgr(nullptr),
    _entityMgr(nullptr),
	_root(nullptr),
	_resourcesCfg(Ogre::BLANKSTRING),
	_pluginsCfg(Ogre::BLANKSTRING)
{
    Logger::init(options.logFile, options.suppressOgreLog);
    Events::Dispatcher::subscribe<Events::Quit>(*this);
}

void Game::run()
{
	// Create and configure the OGRE root object 
	_resourcesCfg = "resources.cfg";
	_pluginsCfg = "plugins.cfg";
	_root = new Ogre::Root(_pluginsCfg);
    
    // Configure our resource directories
	parseOgreResourcesConfig();
    
    // Load main OGRE config, or show the config dialog if none exists
    //if (_options.showConfigDialog || !_root->restoreConfig())
        LOG_DEBUG << _root->showConfigDialog();
    //exit();

    _window = new Window();
    _inputMgr = new InputManager();
    _entityMgr = new EntityManager();
    
    debugSetup();
    _root->startRendering();
    //std::cin.get();
    
    delete _entityMgr;
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

void Game::onEvent(const Events::Quit &event)
{
    _root->queueEndRendering();
}

void Game::parseOgreResourcesConfig()
{
    auto path = fs::current_path();
#ifdef _DEBUG
    // running program from the "build" directory
    path = path.parent_path();
#endif
    path /= "res";
    LOG_INFO << "scanning " << path.string() << " for resources";
    
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path.string(), "FileSystem");
    auto iter = fs::recursive_directory_iterator(path);
    for (const auto &entry : iter)
    {
        if (fs::is_directory(entry.status()))
        {
            //LOG_INFO << "adding directory " << entry.path().string();
            Ogre::ResourceGroupManager::getSingleton().
                addResourceLocation(entry.path().string(), "FileSystem");
        }
    }
    std::cin.get();
}
    
void Game::debugSetup()
{
#if 0
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    
    _sceneMgr = _root->createSceneManager();
    
    _camera = _sceneMgr->createCamera("MainCamera");
    _camera->setNearClipDistance(5);
    auto cameraNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
    cameraNode->attachObject(_camera);
    cameraNode->setPosition(0, 0, 80);
    cameraNode->lookAt(Ogre::Vector3(0, 0, -300), Ogre::SceneNode::TS_WORLD);
    
    auto viewport = _window->_renderWindow->addViewport(_camera);
    viewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
    
    _camera->setAspectRatio(
        Ogre::Real(viewport->getActualWidth()) / Ogre::Real(viewport->getActualHeight()));
    
    //auto locations = Ogre::ResourceGroupManager::getSingleton().
    
    //auto entity = _sceneMgr->createEntity("ogrehead.mesh");
    //auto sceneNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
    //sceneNode->attachObject(entity);
    
    _sceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    
    auto lightNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(_sceneMgr->createLight("MainLight"));
    lightNode->setPosition(20, 80, 50);
#endif
}

bool Game::frameRenderingQueued(const Ogre::FrameEvent &e)
{
}
