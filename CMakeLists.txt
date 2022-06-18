cmake_minimum_required(VERSION 3.21)
project(LabyrinthRomand)

find_package(OpenGL REQUIRED)

add_subdirectory(glfw)

include_directories(glfw/include)
include_directories(glm)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(external/doctest)

add_executable(LabyrinthRomand main.cpp functions.cpp functions.h calculate.cpp calculate.h)

target_link_libraries(LabyrinthRomand
        glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY} )

add_executable(GAME_TEST test.cpp calculate.cpp)

target_link_libraries(GAME_TEST doctest)

enable_testing()


add_test(NAME LabyrinthRomand COMMAND LabyrinthRomand --force-colors -d)

add_custom_target(cloud-test COMMAND LabyrinthRomand --force-colors -d)


file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
