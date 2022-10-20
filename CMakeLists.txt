## This file is a quick tutorial on writing CMakeLists for targeting the Vita
cmake_minimum_required(VERSION 2.8)


## Define project parameters here
# Name of the project
project(hello_world)

## Flags and includes for building
# Note that we make sure not to overwrite previous flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(JSON_INCLUDE_DIR "$ENV{PSPDEV}/psp/include/jsoncpp")

# Add any additional include paths here
include_directories(
  ${JSON_INCLUDE_DIR}
)

## Build and link
# Add all the files needed to compile here
add_executable(${PROJECT_NAME}
  main.cpp
)

# Library to link to (drop the -l prefix). This will mostly be stubs.
target_link_libraries(${PROJECT_NAME}
  jsoncpp
  pspdebug
  pspdisplay
  pspge
)

create_pbp_file(
    TARGET ${PROJECT_NAME}
    ICON_PATH NULL
    BACKGROUND_PATH NULL
    PREVIEW_PATH NULL
    TITLE Hello-Json
    BUILD_PRX
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/message.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
