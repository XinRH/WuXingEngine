cmake_minimum_required(VERSION 3.4.1)

# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")

set(PROJECT_NAME gl2jni)

add_library(${PROJECT_NAME} SHARED
            gl_code.cpp
            gles.h)

string(REGEX REPLACE "(.*)/(.*)/(.*)/(.*)/(.*)/(.*)" "\\1" WUXINGENGINE_PATH  ${CMAKE_CURRENT_SOURCE_DIR})
include(${WUXINGENGINE_PATH}/source/resource/cmake/path.cmake)

include_directories(${SOURCE_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${WUXINGENGINE_PATH}/build/win/inc)

# ADD LIB DEPENDENCIES
include(${RESOURCE_PATH}/generated/cmake/EGL.cmake)
include(${RESOURCE_PATH}/generated/cmake/GLESv2.cmake)
include(${RESOURCE_PATH}/generated/cmake/android.cmake)
include(${RESOURCE_PATH}/generated/cmake/log.cmake)
# add lib dependencies



