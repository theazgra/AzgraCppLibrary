cmake_minimum_required(VERSION 3.5)

project(azgra-test LANGUAGES CXX)

add_executable(test-app test.cpp)

set_property(TARGET test-app PROPERTY CXX_STANDARD 17)

find_package(Catch2 REQUIRED)
target_link_libraries(test-app Catch2::Catch2)
target_link_libraries(test-app azgra::azgra)

include(CTest)
include(Catch)
catch_discover_tests(test-app)
