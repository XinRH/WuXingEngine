# CMAKE
add_definitions(-DWUXING_glm)
add_subdirectory(${3RDPARTYLIBRARY_PATH}/glm ${WUXINGENGINE_PATH}/build/3rdPartyLibrary/glm)
link_libraries(glm)

file(COPY ${3RDPARTYLIBRARY_PATH}/glm/glm/glm.hpp DESTINATION ${WUXINGENGINE_PATH}/build/win/inc PATTERN *.hpp)
# cmake
