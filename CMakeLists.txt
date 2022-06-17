cmake_minimum_required(VERSION 3.21)
project(LabyrinthRomand)

find_package(OpenGL REQUIRED)
#find_package(doctest 2.4.8 REQUIRED)


add_subdirectory(glfw)

include_directories(glfw/include)
include_directories(glm)

set(CMAKE_CXX_STANDARD 14)

add_executable(LabyrinthRomand main.cpp functions.cpp functions.h )

target_link_libraries(LabyrinthRomand
        glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})


#add_test(NAME LabyrinthRomand_test COMMAND LabyrinthRomand_test --force-colors -d)

#add_custom_target(cloud-test COMMAND LabyrinthRomand_test --force-colors -d)