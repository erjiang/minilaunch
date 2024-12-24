cmake_minimum_required(VERSION 3.5)

project(minilaunch)

# Set LTO policy before enabling it
cmake_policy(SET CMP0069 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enhanced optimization flags for size
set(CMAKE_CXX_FLAGS_RELEASE "-Os -fdata-sections -ffunction-sections -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--gc-sections -Wl,-s -Wl,--as-needed")

# Enable LTO in release mode
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Find X11 package for the launcher
find_package(X11 REQUIRED)

add_executable(minilaunch main.cpp launchmath.cpp units.cpp)
target_link_libraries(minilaunch ${X11_LIBRARIES})
target_include_directories(minilaunch PRIVATE ${X11_INCLUDE_DIR})
