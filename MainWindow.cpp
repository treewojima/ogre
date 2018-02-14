#include "MainWindow.h"

MainWindow::MainWindow() : OgreBites::ApplicationInstance("OgreApp")
{
}

void MainWindow::setup()
{
	OgreBites::ApplicationContext::setup();

	addInputListener(this);

	auto root = getRoot();
	auto scnMgr = root->createSceneManager();
	Ogre::RTShader::ShaderGenerator::getSingleton().addSceneManager(scnMgr);

	auto lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	lightNode->setPosition(0, 10, 15);
	lightNode->attachObject(scnMgr->createLight("MainLight"));

	auto camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	camNode->setPosition(0, 0, 15);
	camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);
	auto cam = scnMgr->createCamera("MainCamera");
	cam->setNearClipDistance(5);
	cam->setAutoAspectRatio(true);
	camNode->attachObject(cam);

	getRenderWindow()->addViewpoint(cam);

	auto node = scnMgr->getRootSceneNode()->createChildSceneNode();
	node->scale(0.25, 0.25, 0.25);
	node->attachObject(scnMgr->createEntity("ogrehead.mesh"));
}

bool MainWindow::keyPressed(const OgreBites::KeyboardEvent &e)
{
	if (e.keysym.sym == SDLK_ESCAPE) getRoot()->queueEndRendering();
	return true;
}

