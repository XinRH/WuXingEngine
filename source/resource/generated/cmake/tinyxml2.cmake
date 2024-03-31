# CMAKE
add_definitions(-DWUXING_tinyxml2)
add_subdirectory(${3RDPARTYLIBRARY_PATH}/tinyxml2 ${WUXINGENGINE_PATH}/build/3rdPartyLibrary/tinyxml2)
link_libraries(tinyxml2)

file(COPY ${3RDPARTYLIBRARY_PATH}/tinyxml2/tinyxml2.h DESTINATION ${WUXINGENGINE_PATH}/build/win/inc/FILES_MATCHING PATTERN *.h)
# cmake
