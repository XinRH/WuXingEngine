# CMAKE
add_definitions(-DWUXING_glad)
add_subdirectory(${3RDPARTYLIBRARY_PATH}/glad ${WUXINGENGINE_PATH}/build/3rdPartyLibrary/glad)
link_libraries(glad)

file(COPY ${3RDPARTYLIBRARY_PATH}/glad/include/glad/glad.h DESTINATION ${WUXINGENGINE_PATH}/build/win/inc/FILES_MATCHING PATTERN *.h)
# cmake
