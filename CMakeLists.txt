cmake_minimum_required(VERSION 3.16.0)

project(azgra CXX)

## Enable Catch2 tests
option(AZGRA_TEST "Compile tests" OFF)

set(OBJECTS_TO_BUILD src/utilities/binary_converter.cpp
        src/io/stream/in_binary_stream_base.cpp
        src/io/stream/in_binary_file_stream.cpp
        src/io/stream/out_binary_stream_base.cpp
        src/io/stream/out_binary_file_stream.cpp
        src/io/stream/out_binary_buffer_stream.cpp
        src/io/stream/in_binary_buffer_stream.cpp
        src/io/stream/memory_bit_stream.cpp
        src/utilities/stopwatch.cpp
        src/utilities/z_order.cpp
        src/string/ascii_string.cpp
        src/utilities/guid.cpp
        src/fs/file_info.cpp
        src/fs/directory_info.cpp
        src/fs/path.cpp
        src/cli/cli_arguments.cpp
        include/azgra/utilities/histogram.h
        include/azgra/string/smart_string_view.h
        include/azgra/utilities/print_helpers.h
        include/azgra/collection/set_utilities.h
        include/azgra/collection/vector_linq.h
        include/azgra/collection/vector_utilities.h
        include/azgra/io/text_file_functions.h
        include/azgra/matrix.h
        include/azgra/collection/enumerable.h
        src/geometry/plot.cpp
        src/io/binary_file_functions.cpp src/io/text_file_functions.cpp)

# Create static library target
add_library(azgra STATIC ${OBJECTS_TO_BUILD})
target_include_directories(azgra PUBLIC include)
set_property(TARGET azgra PROPERTY CXX_STANDARD 17)

# Compiler flags
target_compile_options(azgra PUBLIC -Wall -fPIC -Wpedantic)
# Linker flags
target_link_options(azgra PUBLIC "")

# This is required for old compilers without std::fs support
# target_link_libraries(azgra PUBLIC stdc++fs)

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS locale)
if (Boost_FOUND)
    target_include_directories(azgra PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(azgra PRIVATE ${Boost_LIBRARIES})
    message("[AZGRA]: Linked required Boost libraries")
endif()

add_library(sub::azgra ALIAS ${PROJECT_NAME})


add_executable(consoleApp main.cpp)
target_link_libraries(consoleApp azgra)
set_property(TARGET consoleApp PROPERTY CXX_STANDARD 17)

if (AZGRA_TEST)

    add_executable(azgra-test tests/test.cpp tests/matrix_test.cpp)
    set_property(TARGET azgra-test PROPERTY CXX_STANDARD 17)

    find_package(Catch2 REQUIRED)
    target_link_libraries(azgra-test PRIVATE Catch2::Catch2)
    target_link_libraries(azgra-test PRIVATE azgra)

    include(CTest)
    include(Catch)
    catch_discover_tests(azgra-test)
endif()
