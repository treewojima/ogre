/* game
* Copyright (C) 2014-2018 Scott Bishop <treewojima@gmail.com>
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

#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include "defines.h"

#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <OgreLog.h>

// Object tag/property that prevents it from being logged
namespace Debug
{
    struct DoNotLog {};
}

namespace Logger
{
    // Message severity types
    typedef const char *SeverityType;
    namespace Severity
    {
        extern SeverityType Plain;
        extern SeverityType Debug;
        extern SeverityType Info;
        extern SeverityType Warning;
        extern SeverityType Error;
        
        namespace Ogre
        {
            extern SeverityType Trivial;
            extern SeverityType Normal;
            extern SeverityType Warning;
            extern SeverityType Critical;
        }
    }

    // Init/shutdown
    void init(const std::string &logFile, bool suppressOgreLog);
    void destroy();

    // Predicate that checks if an object is flagged as "do not log"
    template <class T>
    bool shouldLog()
    {
        return !std::is_base_of<Debug::DoNotLog, T>();
    }
    
    // LogListener to connect OGRE's logging system to the main logger
    class OgreLogListener : public Ogre::LogListener
    {
    public:
        void messageLogged(const Ogre::String &message,
                           Ogre::LogMessageLevel lml,
                           bool maskDebug,
                           const Ogre::String &logName,
                           bool &skipThisMessage) override;
    };
}

BOOST_LOG_GLOBAL_LOGGER(gLog, boost::log::sources::severity_logger_mt<Logger::SeverityType>)

#define LOG_BASE(s) BOOST_LOG_SEV(gLog::get(), s)

#define LOG_PLAIN   LOG_BASE(Logger::Severity::Plain)
#define LOG_DEBUG   LOG_BASE(Logger::Severity::Debug)
#define LOG_INFO    LOG_BASE(Logger::Severity::Info)
#define LOG_WARNING LOG_BASE(Logger::Severity::Warning)
#define LOG_ERROR   LOG_BASE(Logger::Severity::Error)

#define LOG_OGRE_TRIVIAL  LOG_BASE(Logger::Severity::Ogre::Trivial)
#define LOG_OGRE_NORMAL   LOG_BASE(Logger::Severity::Ogre::Normal)
#define LOG_OGRE_WARNING  LOG_BASE(Logger::Severity::Ogre::Warning)
#define LOG_OGRE_CRITICAL LOG_BASE(Logger::Severity::Ogre::Critical)

#endif
