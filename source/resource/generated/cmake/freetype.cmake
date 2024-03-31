# CMAKE
add_definitions(-DWUXING_freetype)
add_subdirectory(${3RDPARTYLIBRARY_PATH}/freetype ${WUXINGENGINE_PATH}/build/3rdPartyLibrary/freetype)
link_libraries(freetype)

file(COPY ${3RDPARTYLIBRARY_PATH}/freetype/include/ft2build.h DESTINATION ${WUXINGENGINE_PATH}/build/win/inc/FILES_MATCHING PATTERN *.h)
# cmake
