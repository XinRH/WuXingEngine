# cmake version
cmake_minimum_required(VERSION 3.28.2) 

# project
set(PROJECT_NAME preCompile)
project(${PROJECT_NAME})

# path & message
string(REGEX REPLACE "(.*)/(.*)" "\\1" WUXINGENGINE_PATH  ${CMAKE_CURRENT_SOURCE_DIR})
include(${WUXINGENGINE_PATH}/source/resource/cmake/path.cmake)

# FILE LIST
file(GLOB MAIN_FILES *.cpp)
file(GLOB_RECURSE FUNCTION_FILES ${FUNCTION_PATH}/*)
SET(ALL_FILES ${MAIN_FILES})
list(APPEND ALL_FILES ${FUNCTION_FILES})

# exe
add_executable(${PROJECT_NAME} ${ALL_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCE_PATH})
target_include_directories(${PROJECT_NAME} PUBLIC ${3RDPARTYLIBRARY_PATH})

# default path
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${WUXINGENGINE_PATH}/build/win/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${WUXINGENGINE_PATH}/build/win/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${WUXINGENGINE_PATH}/build/win/bin)

# 3RDPARTYLIBRARY
add_definitions(-DWUXING_tinyxml2)
add_subdirectory(${3RDPARTYLIBRARY_PATH}/tinyxml2 ${WUXINGENGINE_PATH}/build/3rdPartyLibrary/tinyxml2)
target_link_libraries(${PROJECT_NAME} tinyxml2)
# 3rdPartyLibrary

# DEFINITION
ADD_DEFINITIONS(-D_PRECOMPILE_ONLY)
ADD_DEFINITIONS(-D_PRECOMPILE_PATH=\"${PRECOMPILE_PATH}\")
ADD_DEFINITIONS(-D_ENGINE_PATH=\"${ENGINE_PATH}\")
ADD_DEFINITIONS(-D_SOURCE_PATH=\"${SOURCE_PATH}\")
ADD_DEFINITIONS(-D_RESOURCE_PATH=\"${RESOURCE_PATH}\")
ADD_DEFINITIONS(-D_ANDROID_PATH=\"${ANDROID_PATH}\")
ADD_DEFINITIONS(-D_COMPILELIBS_PATH=\"${COMPILELIBS_PATH}\")
