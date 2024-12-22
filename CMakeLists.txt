cmake_minimum_required(VERSION 3.5)

project(Launcher)

set(CMAKE_CXX_STANDARD 11)

# Add optimization flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")  # Enable Link Time Optimization
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bsymbolic-functions")  # Enable symbol resolution optimization

# For MSVC (Windows), we need different flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GL")  # Enable whole program optimization
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")  # Enable link-time code generation
endif()

# Find X11 package for the launcher
find_package(X11 REQUIRED)

add_executable(Launcher main.cpp launchmath.cpp units.cpp)
target_link_libraries(Launcher ${X11_LIBRARIES})
target_include_directories(Launcher PRIVATE ${X11_INCLUDE_DIR})

# If you want to keep the tests, you'll need to switch to a different testing framework
# Here's an example using Google Test (you would need to find/install it first):
# find_package(GTest REQUIRED)
#
# add_executable(units_test
#     tests/units_test.cpp
#     units.cpp
# )
# target_link_libraries(units_test GTest::GTest GTest::Main)
#
# add_executable(launchmath_test
#     tests/launchmath_test.cpp
#     launchmath.cpp
# )
# target_link_libraries(launchmath_test GTest::GTest GTest::Main)
#
# enable_testing()
# add_test(NAME units_test COMMAND units_test)
# add_test(NAME launchmath_test COMMAND launchmath_test)
