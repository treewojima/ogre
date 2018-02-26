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

#include "logger.h"

#include <boost/core/null_deleter.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup.hpp>
#include <fstream>
#include <iostream>
#include <OgreLogManager.h>

namespace bl = boost::log;
namespace attr = bl::attributes;
namespace expr = bl::expressions;
namespace src = bl::sources;

namespace
{
    bool _suppressOgreLog;
    void initOgreLog();
    Logger::OgreLogListener *getLogListener();
}

namespace Logger::Severity
{
    // It's a bit cludgy to include formatting in the SeverityType strings,
    // but it works

    SeverityType Plain   = " ";
    SeverityType Debug   = " <Debug>   ";
    SeverityType Info    = " <Info>    ";
    SeverityType Warning = " <Warning> ";
    SeverityType Error   = " <Error>   ";
    
    namespace Ogre
    {
        SeverityType Trivial  = " <OGRE/Trivial>  ";
        SeverityType Normal   = " <OGRE/Normal>   ";
        SeverityType Warning  = " <OGRE/Warning>  ";
        SeverityType Critical = " <OGRE/Critical> ";
    }
}

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", Logger::SeverityType)

BOOST_LOG_GLOBAL_LOGGER_INIT(gLog, src::severity_logger_mt<Logger::SeverityType>)
{
    src::severity_logger_mt<Logger::SeverityType> lg;
    return lg;
}

void Logger::init(const std::string &logFile, bool suppressOgreLog)
{    
    bl::core::get()->add_global_attribute("TimeStamp", attr::local_clock());
    
    typedef bl::sinks::synchronous_sink<bl::sinks::text_ostream_backend> text_sink;
    auto sink = boost::make_shared<text_sink>();

    // File stream
    sink->locked_backend()->add_stream(
                boost::make_shared<std::ofstream>(logFile));

    // std::clog stream
    sink->locked_backend()->add_stream(
                boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));

    // Flush after each log entry
    sink->locked_backend()->auto_flush();

    // Set the log format
    auto formatter = expr::stream
        << "["
        << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%m-%d-%Y %H:%M:%S")
        << "]"
        << expr::attr<Logger::SeverityType>("Severity")
        << expr::smessage;
    sink->set_formatter(formatter);

#ifndef _DEBUG
    sink->set_filter(severity != Logger::Severity::Debug);
#endif

    bl::core::get()->add_sink(sink);
    
    _suppressOgreLog = suppressOgreLog;
    initOgreLog();
}

void Logger::destroy()
{
    bl::core::get()->remove_all_sinks();
}

// Forward all OGRE messages to the default logger
void Logger::OgreLogListener::messageLogged(
    const Ogre::String &message,
    Ogre::LogMessageLevel lml,
    bool maskDebug,
    const Ogre::String &logName,
    bool &skipThisMessage)
{
    if (!_suppressOgreLog)
    {
        switch (lml)
        {
        case Ogre::LML_TRIVIAL:
            LOG_OGRE_TRIVIAL << message;
            break;
            
        case Ogre::LML_NORMAL:
            LOG_OGRE_NORMAL << message;
            break;
            
        case Ogre::LML_WARNING:
            LOG_OGRE_WARNING << message;
            break;
            
        case Ogre::LML_CRITICAL:
            LOG_OGRE_CRITICAL << message;
            break;
            
        default:
            LOG_WARNING << "Unknown OGRE log message: " << message;
        }
    }
    
    skipThisMessage = true;
}

namespace {
    
void initOgreLog()
{
    // First, create a LogManager and custom log, suppressing file output
    auto logMgr = new Ogre::LogManager();
    logMgr->setLogDetail(Ogre::LL_BOREME);
    auto log = logMgr->createLog("ogre.log", true, true, true);
    
    // Next, create the custom listener to act as a sink for OGRE messages
    log->addListener(getLogListener());
}

Logger::OgreLogListener *getLogListener()
{
    static Logger::OgreLogListener listener;
    return &listener;
}

}
