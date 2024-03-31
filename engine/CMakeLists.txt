# cmake version
cmake_minimum_required(VERSION 3.28.2)

# COMPILER
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
# compiler

# project
set(PROJECT_NAME engine)
project(${PROJECT_NAME})

# path
string(REGEX REPLACE "(.*)/(.*)" "\\1" WUXINGENGINE_PATH  ${CMAKE_CURRENT_SOURCE_DIR})
include(${WUXINGENGINE_PATH}/source/resource/cmake/path.cmake)

# FILE LIST
file(GLOB MAIN_FILES *.cpp)
file(GLOB_RECURSE CORE_FILES ${CORE_PATH}/*)
file(GLOB_RECURSE FUNCTION_FILES ${FUNCTION_PATH}/*)

SET(ALL_FILES ${MAIN_FILES})
list(APPEND ALL_FILES ${CORE_FILES})
list(APPEND ALL_FILES ${FUNCTION_FILES})

# exe
add_executable(${PROJECT_NAME} ${ALL_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCE_PATH})
target_include_directories(${PROJECT_NAME} PUBLIC ${ANDROID_PATH}/app/src/main/cpp)

# LINK
add_library(tinyxml2 STATIC IMPORTED)
set_target_properties(tinyxml2 PROPERTIES IMPORTED_LOCATION ${WUXINGENGINE_PATH}/build/win/lib/libtinyxml2.a)
target_link_libraries(${PROJECT_NAME} tinyxml2)

add_library(freetype STATIC IMPORTED)
set_target_properties(freetype PROPERTIES IMPORTED_LOCATION ${WUXINGENGINE_PATH}/build/win/lib/libfreetype.a)
target_link_libraries(${PROJECT_NAME} freetype)

add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${WUXINGENGINE_PATH}/build/win/lib/libglfw3.a)
target_link_libraries(${PROJECT_NAME} glfw)

add_library(SDL2 MODULE IMPORTED)
set_target_properties(SDL2 PROPERTIES IMPORTED_LOCATION ${WUXINGENGINE_PATH}/build/win/bin/SDL2.dll)
target_link_libraries(${PROJECT_NAME} SDL2)

add_library(glad STATIC IMPORTED)
set_target_properties(glad PROPERTIES IMPORTED_LOCATION ${WUXINGENGINE_PATH}/build/win/lib/libglad.a)
target_link_libraries(${PROJECT_NAME} glad)

include(${RESOURCE_PATH}/generated/cmake/OpenGL.cmake)

include(${RESOURCE_PATH}/generated/cmake/EnTT.cmake)

add_library(glm STATIC IMPORTED)
set_target_properties(glm PROPERTIES IMPORTED_LOCATION ${WUXINGENGINE_PATH}/build/win/lib/libglm.a)
target_link_libraries(${PROJECT_NAME} glm)

# link
target_include_directories(${PROJECT_NAME} PUBLIC ${WUXINGENGINE_PATH}/build/win/inc)

# DEFINITION
ADD_DEFINITIONS(-D_ENGINE_PATH=\"${ENGINE_PATH}\")
ADD_DEFINITIONS(-D_PRECOMPILE_PATH=\"${PRECOMPILE_PATH}\")
ADD_DEFINITIONS(-D_RESOURCE_PATH=\"${RESOURCE_PATH}\")
