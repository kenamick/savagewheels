cmake_minimum_required (VERSION 2.6)
project(savagewheels) 

### check for required libs
find_package(SDL REQUIRED)

### src
include_directories(src/)
include_directories(tinyxml/)
#include_directories(fmod/linux/api)
include_directories(fmod/linux/api/inc)

set (
        SOURCES
        src/Bindings.cpp 
	src/CDeadToy.cpp 
	src/CKdf.cpp 
	src/Csdl.cpp 
	src/Cswv_module.cpp 
	src/Cvehicle.cpp 
	src/Utils.cpp 
	src/CAnimation.cpp 
	src/Cgame.cpp 
	src/CMainMenu.cpp 
	src/CSounds.cpp
	src/CTimer.cpp
	src/Main.cpp
	tinyxml/tinystr.cpp
	tinyxml/tinyxml.cpp
	tinyxml/tinyxmlerror.cpp
	tinyxml/tinyxmlparser.cpp	
)

### specify executable

add_executable (	
	savagewheels
	${SOURCES}
)

### add required libarires

target_link_libraries( 
	savagewheels
	${SDL_LIBRARY}
	SDLmain
)

set( FMOD_LIB_NAME "libfmod-3.75.so" )

find_library( 
  FMOD_LIBRARY
  NAMES ${FMOD_LIB_NAME}
  PATHS "./fmod/linux/api" )

if ( NOT FMOD_LIBRARY OR NO_SOUND )
  message( STATUS "FMod lib(${FMOD_LIB_NAME}) not found. Will compile without sound support." )
else ( NOT FMOD_LIBRARY OR NO_SOUND )
  add_definitions(-DWITH_FMOD)
  target_link_libraries(savagewheels ${FMOD_LIBRARY})
  message( STATUS "FMod lib(${FMOD_LIB_NAME}) at ${FMOD_LIBRARY}." )
endif ( NOT FMOD_LIBRARY OR NO_SOUND )

### additional defines

add_definitions(`sdl-config --cflags`)
add_definitions(`sdl-config --libs`)
add_definitions(-DLINUX_BUILD)

message( STATUS "Build type is ${CMAKE_BUILD_TYPE}" )

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-g)
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-O3 -DNDEBUG)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

set(savagewheels PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin/")
