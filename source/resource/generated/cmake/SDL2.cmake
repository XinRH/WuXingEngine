# CMAKE
add_definitions(-DWUXING_SDL2)
add_subdirectory(${3RDPARTYLIBRARY_PATH}/SDL2 ${WUXINGENGINE_PATH}/build/3rdPartyLibrary/SDL2)
link_libraries(SDL2)

file(COPY ${3RDPARTYLIBRARY_PATH}/SDL2/include/SDL.h DESTINATION ${WUXINGENGINE_PATH}/build/win/inc PATTERN *.h)
# cmake
