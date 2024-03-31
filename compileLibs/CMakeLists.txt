# cmake version
cmake_minimum_required(VERSION 3.28.2)

# project
set(PROJECT_NAME compileLibs)
project(${PROJECT_NAME})

# path
string(REGEX REPLACE "(.*)/(.*)" "\\1" WUXINGENGINE_PATH  ${CMAKE_CURRENT_SOURCE_DIR})
include(${WUXINGENGINE_PATH}/source/resource/cmake/path.cmake)

# default path
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${WUXINGENGINE_PATH}/build/win/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${WUXINGENGINE_PATH}/build/win/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${WUXINGENGINE_PATH}/build/win/bin)

# 3RDPARTYLIBRARY
include(${RESOURCE_PATH}/generated/cmake/tinyxml2.cmake)
include(${RESOURCE_PATH}/generated/cmake/freetype.cmake)
include(${RESOURCE_PATH}/generated/cmake/glfw.cmake)
include(${RESOURCE_PATH}/generated/cmake/glad.cmake)
include(${RESOURCE_PATH}/generated/cmake/EnTT.cmake)
include(${RESOURCE_PATH}/generated/cmake/glm.cmake)
# 3rdPartyLibrary




