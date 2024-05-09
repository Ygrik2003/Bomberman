cmake_minimum_required(VERSION 3.29.0)

find_package(SDL2 REQUIRED)

file(GLOB_RECURSE bomberman_src CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/*.cpp
                                                  ${CMAKE_CURRENT_LIST_DIR}/*.hpp
                                                  ${CMAKE_CURRENT_LIST_DIR}/*.c
                                                  ${CMAKE_CURRENT_LIST_DIR}/*.h)

add_executable(Bomberman ${bomberman_src})
target_link_libraries(Bomberman PRIVATE SDL2::SDL2)
target_include_directories(Bomberman PRIVATE ${CMAKE_CURRENT_LIST_DIR})

add_custom_command(
    TARGET Bomberman
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:Bomberman>/res
)
