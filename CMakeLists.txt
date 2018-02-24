cmake_minimum_required(VERSION 2.8.3)

set(PROJECT_NAME OgreGame)
project(${PROJECT_NAME})

set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 1)
set(${PROJECT_NAME}_PATCH_VERSION 0)
set(${PROJECT_NAME}_VERSION
	${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_VERSION})
set(PROJECT_VERSION ${${PROJECT_NAME}_VERSION})

set(${PROJECT_NAME}_INCLUDE_DIR "include")
#set(${PROJECT_NAME}_SRC_DIR "src")

find_package(Boost 1.56 REQUIRED
			 COMPONENTS filesystem log program_options system thread)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR_DEBUG})

set(CMAKE_MODULE_PATH "C:/Users/SFSVCADV20/Documents/Scott/ogre-build/sdk/CMake")
set(OGRE_DIR "C:/Users/SFSVCADV20/Documents/Scott/ogre-build/sdk/CMake")
find_package(OGRE 1.10 REQUIRED)
include_directories(${OGRE_INCLUDE_DIRS})
link_directories(${OGRE_LIBRARY_DIRS})
file(COPY ${OGRE_CONFIG_DIR}/plugins.cfg ${OGRE_CONFIG_DIR}/resources.cfg
	DESTINATION ${CMAKE_BINARY_DIR})

set(OIS_INCLUDE_DIR "/mingw64/include/ois")
include_directories(${OIS_INCLUDE_DIR})
    
include_directories(${${PROJECT_NAME}_INCLUDE_DIR})
set(${PROJECT_NAME}_SRC_FILES
    src/game.cpp
    src/inputmanager.cpp
    src/logger.cpp
    src/main.cpp
    src/window.cpp)

#get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#foreach(dir ${dirs})
#  message(STATUS "dir='${dir}'")
#endforeach()

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC_FILES})
target_link_libraries(${PROJECT_NAME}
	${OGRE_LIBRARIES}
	${Boost_FILESYSTEM_LIBRARY_DEBUG}
	${Boost_LOG_LIBRARY_DEBUG}
	${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG}
	${Boost_SYSTEM_LIBRARY_DEBUG}
	${Boost_THREAD_LIBRARY_DEBUG}
    libOIS.dll.a)

