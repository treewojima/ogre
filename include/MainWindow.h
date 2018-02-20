#ifndef __OGRE_MAINWINDOW_H__
#define __OGRE_MAINWINDOW_H__

#include <Ogre.h>
#include <OgreApplicationContext.h>

class MainWindow : public OgreBites::ApplicationContext, 
				   public OgreBites::InputListener
{
public:
	MainWindow();
	void setup() override;
	bool keyPressed(const OgreBites::KeyboardEvent &e) override;
};

#endif

