cmake_minimum_required(VERSION 3.21)
project(LabyrinthRomand)

find_package(OpenGL REQUIRED)


add_subdirectory(glfw)

include_directories(glfw/include)
include_directories(glm)

set(CMAKE_CXX_STANDARD 14)

add_executable(LabyrinthRomand main.cpp functions.cpp functions.h)

target_link_libraries(LabyrinthRomand
        glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY} doctest)

enable_testing()

add_subdirectory(external/doctest)

add_test(NAME LabyrinthRomand COMMAND LabyrinthRomand --force-colors -d)

add_custom_target(cloud-test COMMAND LabyrinthRomand --force-colors -d)

