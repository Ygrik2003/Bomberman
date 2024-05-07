cmake_minimum_required(VERSION 3.0.0)
project(Bomberman VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(src)

# file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/src/res)

# file(COPY res/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/src/res)
# install(DIRECTORY res/ DESTINATION ${CMAKE_INSTALL_PREFIX}/src/res)