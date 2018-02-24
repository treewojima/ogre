#ifndef __OGRE_INPUTMANAGER_H__
#define __OGRE_INPUTMANAGER_H__

#include "defines.h"

#include <OgreFrameListener.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

class InputManager : public Ogre::WindowEventListener, public Ogre::FrameListener
{
    friend class Window;
    
public:
    InputManager();
    ~InputManager();
    
    const inline OIS::MouseState &getMouseState() const
        { return _mouse->getMouseState(); }
    
private:
    OIS::InputManager *_inputMgr;
    OIS::Keyboard *_keyboard;
    OIS::Mouse *_mouse;
    
    void shutdown();
    void windowResized(Ogre::RenderWindow *renderWindow) override;
    void windowClosed(Ogre::RenderWindow *renderWindow) override;
    bool frameRenderingQueued(const Ogre::FrameEvent &e) override;
};

#endif
