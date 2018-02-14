cmake_minimum_required(VERSION 2.8.3)

set(PROJECT_NAME OgreGame)
project(${PROJECT_NAME})

set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 1)
set(${PROJECT_NAME}_PATCH_VERSION 0)
set(${PROJECT_NAME}_VERSION
	${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_VERSION})
set(PROJECT_VERSION ${${PROJECT_NAME}_VERSION})

set(CMAKE_MODULE_PATH "C:/Users/SFSVCADV20/Documents/Scott/ogre-build/sdk/CMake")

find_package(OGRE 1.10 REQUIRED)
include_directories(${OGRE_INCLUDE_DIRS})
link_directories(${OGRE_LIBRARY_DIRS})
file(COPY ${OGRE_CONFIG_DIR}/plugins.cfg ${OGRE_CONFIG_DIR}/resources.cfg
	DESTINATION ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} MainWindow.cpp main.cpp)
target_link_libraries(${PROJECT_NAME} ${OGRE_LIBRARIES})

