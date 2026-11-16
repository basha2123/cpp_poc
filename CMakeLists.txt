cmake_minimum_required(VERSION 3.16)
project(EmployeeManagementPOC LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Enable gcov coverage flags" OFF)

find_package(SQLite3 REQUIRED)
find_package(GTest REQUIRED)

add_library(employee_core
    src/EmployeeRepository.cpp
    src/EmployeeService.cpp
)
target_include_directories(employee_core PUBLIC include)
target_link_libraries(employee_core PUBLIC SQLite::SQLite3)

add_executable(employee_app src/main.cpp)
target_link_libraries(employee_app PRIVATE employee_core)

enable_testing()
add_executable(employee_tests tests/EmployeeTests.cpp)
target_link_libraries(employee_tests PRIVATE employee_core GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(employee_tests)

if(ENABLE_COVERAGE)
    foreach(target employee_core employee_tests)
        target_compile_options(${target} PRIVATE --coverage -O0 -g)
        target_link_options(${target} PRIVATE --coverage)
    endforeach()
endif()
