cmake_minimum_required(VERSION 3.5)

project(Launcher)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

# Add optimization flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")  # Enable Link Time Optimization
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bsymbolic-functions")  # Enable symbol resolution optimization

# For MSVC (Windows), we need different flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GL")  # Enable whole program optimization
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")  # Enable link-time code generation
endif()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

add_executable(Launcher main.cpp launchmath.cpp units.cpp)
target_link_libraries(Launcher Qt5::Widgets)

add_executable(units_test
    tests/units_test.cpp
    units.cpp
)
target_link_libraries(units_test
    Qt5::Core
    Qt5::Test
)

add_executable(launchmath_test
    tests/launchmath_test.cpp
    launchmath.cpp
)
target_link_libraries(launchmath_test
    Qt5::Core
    Qt5::Test
)

enable_testing()
add_test(NAME units_test COMMAND units_test)
add_test(NAME launchmath_test COMMAND launchmath_test)
