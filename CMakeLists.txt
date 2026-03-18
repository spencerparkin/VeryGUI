# CMakeLists.txt for the VeryGUI project.

cmake_minimum_required(VERSION 3.5)

project(VeryGUI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Library)
add_subdirectory(Test)
add_subdirectory(ThirdParty)