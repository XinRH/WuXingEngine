# CMAKE
add_definitions(-DWUXING_OpenGL)
find_package(OpenGL REQUIRED)
link_libraries(${OPENGL_LIBRARIES})
# cmake
