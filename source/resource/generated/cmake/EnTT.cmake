# CMAKE
add_definitions(-DWUXING_EnTT)
add_subdirectory(${3RDPARTYLIBRARY_PATH}/EnTT ${WUXINGENGINE_PATH}/build/3rdPartyLibrary/EnTT)
link_libraries(EnTT)

file(COPY ${3RDPARTYLIBRARY_PATH}/EnTT/single_include/entt/entt.hpp DESTINATION ${WUXINGENGINE_PATH}/build/win/inc/FILES_MATCHING PATTERN *.hpp)
# cmake
