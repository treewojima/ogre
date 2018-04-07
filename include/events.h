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

#ifndef __OGRE_EVENTS_H__
#define __OGRE_EVENTS_H__

#include "defines.h"

#include <boost/core/demangle.hpp>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>

#include "stringable.h"

#ifdef _DEBUG
#   define _DEBUG_EVENTS
#   include "logger.h"
#endif

// Let's not indent the entire header file just for this...
namespace Events {

// Base event class
class Event : public Stringable
{
//protected:
public:
    Event() {}
    
public:
    virtual ~Event() {}
};

// Must guarantee the following function:
// (virtual) void onEvent(const <subclass of Event> &event)
class Subscriber {};

// Must guarantee the following function:
// (virtual) void queueEvent(std::shared_ptr<subclass of Event> event)
class AsyncSubscriber {};
 
// Master dispatcher class
class Dispatcher
{
private:
    Dispatcher() {}

public:
    template <class E,
              class T,
              typename std::enable_if<
                  std::is_base_of<Subscriber, T>::value>::type* = nullptr>
    static void subscribe(T &subscriber)
    {
        static_assert(std::is_base_of<Event, E>::value,
                      "Can only subscribe to types derived from class Events::Base");
        //static_assert(std::is_base_of<Subscriber, T>::value,
        //              "Only subclasses of class Subscriber can subscribe to synchronous events");
#ifdef _DEBUG_EVENTS
        static_assert(std::is_base_of<Stringable, T>::value,
                      "Only subclasses of class Stringable can subscribe to synchronous events " \
                      "when debugging is enabled");
#endif

        subscribeSync<E>(subscriber);
    }

    template <class E,
              class T,
              typename std::enable_if<
                  std::is_base_of<AsyncSubscriber, T>::value>::type* = nullptr>
    static void subscribe(T &subscriber)
    {
        static_assert(std::is_base_of<Event, E>::value,
                      "Can only subscribe to types derived from class Events::Base");
        //static_assert(std::is_base_of<AsyncSubscriber, T>::value,
        //              "Only subclasses of class Subscriber can subscribe to asynchronous events");
#ifdef _DEBUG_EVENTS
        static_assert(std::is_base_of<Stringable, T>::value,
                      "Only subclasses of class Stringable can subscribe to asynchronous events " \
                      "when debugging is enabled");
#endif

        subscribeAsync<E>(subscriber);
    }

    template <class E,
              class T,
              typename std::enable_if<
                  std::is_base_of<Subscriber, T>::value>::type* = nullptr>
    static void unsubscribe(T &subscriber)
    {
        static_assert(std::is_base_of<Event, E>::value,
                      "Can only unsubscribe from types derived from class Events::Base");

        unsubscribeSync<E>(subscriber);
    }

    template <class E,
              class T,
              typename std::enable_if<
                  std::is_base_of<AsyncSubscriber, T>::value>::type* = nullptr>
    static void unsubscribe(T &subscriber)
    {
        static_assert(std::is_base_of<Event, E>::value,
                      "Can only unsubscribe from types derived from class Events::Base");

        unsubscribeAsync<E>(subscriber);
    }

    template <class T,
              typename std::enable_if<
                  std::is_base_of<Subscriber, T>::value>::type* = nullptr>
    static void unsubscribe(T &subscriber)
    {
        unsubscribeSync(subscriber);
    }

    template <class T,
              typename std::enable_if<
                  std::is_base_of<AsyncSubscriber, T>::value>::type* = nullptr>
    static void unsubscribe(T &subscriber)
    {
        unsubscribeAsync(subscriber);
    }

    template <class E, class... Args>
    static void raise(Args&& ... args);

private:
    template <class E, class T>
    static void subscribeSync(T &subscriber);

    template <class E, class T>
    static void subscribeAsync(T &subscriber);

    template <class E, class T>
    static void unsubscribeSync(T &subscriber);

    template <class E, class T>
    static void unsubscribeAsync(T &subscriber);

    template <class T>
    static void unsubscribeSync(T &subscriber);

    template <class T>
    static void unsubscribeAsync(T &subscriber);

    typedef std::function<void(const Event &)> SubscriberCallback;
    typedef std::pair<Subscriber *, SubscriberCallback> SubscriberPair;
    typedef std::list<SubscriberPair> SubscriberList;
    typedef std::unordered_map<std::type_index, SubscriberList> TypeSubscriberMap;

    typedef std::function<void(std::shared_ptr<Event>)> AsyncSubscriberCallback;
    typedef std::pair<AsyncSubscriber *, AsyncSubscriberCallback> AsyncSubscriberPair;
    typedef std::list<AsyncSubscriberPair> AsyncSubscriberList;
    typedef std::unordered_map<std::type_index, AsyncSubscriberList> TypeAsyncSubscriberMap;

    static TypeSubscriberMap _map;
    static TypeAsyncSubscriberMap _asyncMap;
};

template <class E, class... Args>
void Dispatcher::raise(Args&& ... args)
{
    static_assert(std::is_base_of<Event, E>::value,
                  "Can only raise types derived from class Events::Base");

    auto event = std::make_shared<E>(std::forward<Args>(args)...);
    auto baseEvent = std::static_pointer_cast<Event>(event);

#ifdef _DEBUG_EVENTS
    if (Logger::shouldLog<E>())
    {
        LOG_DEBUG << "event raised: " << boost::core::demangle(typeid(E).name());
    }
#endif

    // First, queue it for asynchronous subscribers
    auto asyncList = _asyncMap[typeid(E)];
    for (auto &pair : asyncList)
    {
        pair.second(baseEvent);
    }

    // Next, fire off regular subscribers
    auto list = _map[typeid(E)];
    for (auto &pair : list)
    {
        pair.second(*baseEvent);
    }
}

template <class E, class T>
void Dispatcher::subscribeSync(T &subscriber)
{
    auto callback = [&subscriber](const Event &event)
        { subscriber.onEvent(static_cast<const E &>(event)); };

    _map[typeid(E)].push_back(
                std::make_pair(static_cast<Subscriber *>(&subscriber),
                               callback));

#ifdef _DEBUG_EVENTS
    LOG_DEBUG << "subscriber " << subscriber
              << " is listening for events of type "
              << boost::core::demangle(typeid(E).name());
#endif
}

template <class E, class T>
void Dispatcher::subscribeAsync(T &subscriber)
{
    auto callback = [&subscriber](std::shared_ptr<Event> event)
        { subscriber.queueEvent(std::static_pointer_cast<E>(event)); };

    _asyncMap[typeid(E)].push_back(
                std::make_pair(static_cast<AsyncSubscriber *>(&subscriber),
                               callback));

#ifdef _DEBUG_EVENTS
    LOG_DEBUG << "asynchronous subscriber " << subscriber
              << " is listening for events of type "
              << boost::core::demangle(typeid(E).name());
#endif
}

template <class E, class T>
void Dispatcher::unsubscribeSync(T &subscriber)
{
    auto s = static_cast<Subscriber *>(&subscriber);
    auto i = _map.find(typeid(E));
    if (i != _map.end())
    {
        auto &list = i->second;
        for (auto j = list.begin(); j != list.end(); j++)
        {
            if (j->first == s)
            {
                list.erase(j);
                break;
            }
        }
    }

#ifdef _DEBUG_EVENTS
    LOG_DEBUG << "subscriber " << subscriber
              << " is no longer listening for events of type "
              << boost::core::demangle(typeid(E).name());
#endif
}

template <class E, class T>
void Dispatcher::unsubscribeAsync(T &subscriber)
{
    auto s = static_cast<AsyncSubscriber *>(&subscriber);
    auto i = _asyncMap.find(typeid(E));
    if (i != _asyncMap.end())
    {
        auto &list = i->second;
        for (auto j = list.begin(); j != list.end(); j++)
        {
            if (j->first == s)
            {
                list.erase(j);
                break;
            }
        }
    }

#ifdef _DEBUG_EVENTS
    LOG_DEBUG << "asynchronous subscriber " << subscriber
              << " is no longer listening for events of type "
              << boost::core::demangle(typeid(E).name());
#endif
}

template <class T>
void Dispatcher::unsubscribeSync(T &subscriber)
{
    auto s = static_cast<Subscriber *>(&subscriber);

    for (auto &i : _map)
    {
        auto &list = i.second;
        for (auto j = list.begin(); j != list.end(); j++)
        {
            if (j->first == s)
            {
                list.erase(j);
                break;
            }
        }
    }

#ifdef _DEBUG_EVENTS
    LOG_DEBUG << "subscriber " << subscriber
              << " is no longer listening for events of any type";
#endif
}

template <class T>
void Dispatcher::unsubscribeAsync(T &subscriber)
{
    auto s = static_cast<AsyncSubscriber *>(&subscriber);

    for (auto &i : _asyncMap)
    {
        auto &list = i.second;
        for (auto j = list.begin(); j != list.end(); j++)
        {
            if (j->first == s)
            {
                list.erase(j);
                break;
            }
        }
    }

#ifdef _DEBUG_EVENTS
    LOG_DEBUG << "asynchronous subscriber " << subscriber
              << " is no longer listening for events of any type";
#endif
}

} // namespace Events

#include "helper_events.h"

#endif
