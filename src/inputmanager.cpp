#include "inputmanager.h"

#include <OISEvents.h>

#include "game.h"
#include "logger.h"

InputManager::InputManager() :
        _inputMgr(nullptr),
        _keyboard(nullptr),
        _mouse(nullptr)
{
    auto window = getGame()->getWindow();
    auto renderWindow = window->_renderWindow;
    size_t hwnd = 0;
    renderWindow->getCustomAttribute("WINDOW", &hwnd);
    
    OIS::ParamList pl;
    std::ostringstream ss;
    ss << hwnd;
    pl.insert(std::make_pair(std::string("WINDOW"), ss.str()));
    
    _inputMgr = OIS::InputManager::createInputSystem(pl);
    _keyboard = static_cast<OIS::Keyboard *>(
        _inputMgr->createInputObject(OIS::OISKeyboard, false));
    _mouse = static_cast<OIS::Mouse *>(
        _inputMgr->createInputObject(OIS::OISMouse, false));
        
    Ogre::WindowEventUtilities::addWindowEventListener(renderWindow, this);
    window->windowResized(renderWindow);
    
    LOG_DEBUG << "created InputManager";
}

InputManager::~InputManager()
{
    shutdown();    
    LOG_DEBUG << "destroyed InputManager";
}

void InputManager::shutdown()
{
    if (_inputMgr)
    {
        _inputMgr->destroyInputObject(_mouse);
        _inputMgr->destroyInputObject(_keyboard);
        OIS::InputManager::destroyInputSystem(_inputMgr);
        _inputMgr = nullptr;
    }
}

void InputManager::windowResized(Ogre::RenderWindow *renderWindow)
{    
    int left, top;
    unsigned width, height, depth;
    
    renderWindow->getMetrics(width, height, depth, left, top);
    auto state = getGame()->getInputMgr()->getMouseState();
    state.width = width;
    state.height = height;
}

void InputManager::windowClosed(Ogre::RenderWindow *renderWindow)
{
    if (renderWindow == getGame()->getWindow()->_renderWindow)
        shutdown();
}

bool InputManager::frameRenderingQueued(const Ogre::FrameEvent &e)
{
    if (getGame()->getWindow()->isClosed()) return false;
    
    _keyboard->capture();
    _mouse->capture();
    
    if (_keyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
    
    return true;
}
