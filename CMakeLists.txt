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

set(CMAKE_MODULE_PATH "C:/Users/SFSVCADV20/Documents/Scott/ogre-build/sdk/CMake")
set(OGRE_DIR "C:/Users/SFSVCADV20/Documents/Scott/ogre-build/sdk/CMake")
find_package(OGRE 1.10 REQUIRED)
include_directories(${OGRE_INCLUDE_DIRS})
link_directories(${OGRE_LIBRARY_DIRS})
file(COPY ${OGRE_CONFIG_DIR}/plugins.cfg ${OGRE_CONFIG_DIR}/resources.cfg
	DESTINATION ${CMAKE_BINARY_DIR})

include_directories(${${PROJECT_NAME}_INCLUDE_DIR})
set(${PROJECT_NAME}_SRC_FILES src/MainWindow.cpp src/main.cpp)
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${OGRE_LIBRARIES})

