#include "events.h"

#include <OgreFrameListener.h>

Events::Dispatcher::TypeSubscriberMap Events::Dispatcher::_map;
Events::Dispatcher::TypeAsyncSubscriberMap Events::Dispatcher::_asyncMap;

// Class to catch various OGRE/OIS callbacks and pump them into the event system
#if 0
class EventHelper : public Ogre::FrameListener
{
public:
    bool frameRenderingQueued(const Ogre::FrameEvent &e)
};
#endif
