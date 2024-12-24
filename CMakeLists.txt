cmake_minimum_required(VERSION 3.5)

project(Launcher)

# Set LTO policy before enabling it
cmake_policy(SET CMP0069 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Aggressive optimization flags for size
set(CMAKE_CXX_FLAGS_RELEASE "-Os -fdata-sections -ffunction-sections -fno-rtti")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--gc-sections -Wl,-s")

# Enable LTO in release mode
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Find X11 package for the launcher
find_package(X11 REQUIRED)

add_executable(Launcher main.cpp launchmath.cpp units.cpp)
target_link_libraries(Launcher ${X11_LIBRARIES})
target_include_directories(Launcher PRIVATE ${X11_INCLUDE_DIR})
