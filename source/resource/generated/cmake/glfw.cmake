# CMAKE
add_definitions(-DWUXING_glfw)
add_subdirectory(${3RDPARTYLIBRARY_PATH}/glfw ${WUXINGENGINE_PATH}/build/3rdPartyLibrary/glfw)
link_libraries(glfw)

file(COPY ${3RDPARTYLIBRARY_PATH}/glfw/include/GLFW/glfw3.h DESTINATION ${WUXINGENGINE_PATH}/build/win/inc PATTERN *.h)
# cmake
