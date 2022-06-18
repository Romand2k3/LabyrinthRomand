# CMake generated Testfile for 
# Source directory: D:/cppsha/LabyrinthRomand
# Build directory: D:/cppsha/LabyrinthRomand/cmake-build-debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[LabyrinthRomand]=] "D:/cppsha/LabyrinthRomand/cmake-build-debug/LabyrinthRomand.exe" "--force-colors" "-d")
set_tests_properties([=[LabyrinthRomand]=] PROPERTIES  _BACKTRACE_TRIPLES "D:/cppsha/LabyrinthRomand/CMakeLists.txt;27;add_test;D:/cppsha/LabyrinthRomand/CMakeLists.txt;0;")
subdirs("glfw")
subdirs("external/doctest")
