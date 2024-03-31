set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB GLAD_SRC ${GLAD_DIR}/src/*.c)
add_library(glad STATIC ${GLAD_SRC})
target_include_directories(glad PUBLIC $<BUILD_INTERFACE:${GLAD_DIR}/include>)