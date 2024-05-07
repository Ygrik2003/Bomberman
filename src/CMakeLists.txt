cmake_minimum_required(VERSION 3.0.0)
project(Bomberman VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_COMPILER_WORKS 1)

# find_package(OpenGL)

# if(NOT OPENGL_FOUND)
#     message("there is no OpenGL!")
# endif()

# Find SDL2 package
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

set(BOMBERMAN_FILES
    main.cpp
    game.hpp
    game.cpp
    config.hpp
    glad/glad.c
    core/actionField.hpp
    core/actionField.cpp
    engine/fileReader.hpp
    engine/fileReader.cpp
    engine/logger.hpp
    engine/logger.cpp
    engine/render.hpp
    engine/render.cpp
    engine/shaderProgram.hpp
    engine/shaderProgram.cpp
    engine/stb_image.h
    engine/stb_image.cpp
    )

include(CTest)
enable_testing()

set(EXECUTABLE_NAME Bomberman)

add_executable(${EXECUTABLE_NAME} ${BOMBERMAN_FILES})
# Link SDL2 library
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

# target_link_libraries(Bomberman PRIVATE glfw)


# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/..)

# add_custom_command(
#     TARGET RoguePhys
#     POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:RoguePhys> ${CMAKE_SOURCE_DIR}/..
# )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
