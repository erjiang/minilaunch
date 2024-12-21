cmake_minimum_required(VERSION 3.5)

project(Launcher)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

add_executable(Launcher main.cpp)
target_link_libraries(Launcher Qt5::Widgets)
