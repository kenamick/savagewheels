cmake_minimum_required (VERSION 2.6)
project(swv_builder) 

### src

set (
        SOURCES 
	../../src/Cswv_module.cpp 
	main.cpp
)

### specify executable

add_executable (	
	swv_builder
	${SOURCES}
)

### additional defines

add_definitions(-DLINUX_BUILD)
add_definitions(-DSWV_CREATE)

message( STATUS "Build type is ${CMAKE_BUILD_TYPE}" )

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-g)
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-O3 -DNDEBUG)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

### set runtime directory
#set(swv_builder PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin/")

