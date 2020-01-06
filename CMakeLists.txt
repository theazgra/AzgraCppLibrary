cmake_minimum_required(VERSION 3.0.0)

project(azgra)

option(AZGRA_TEST "Compile tests" ON)
option(MATPLOTLIB_CPP "Compile with matplotlib-cpp support" ON)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -Wpedantic" ) #
# Linker flags
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}")


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

add_library(azgra STATIC ${OBJECTS_TO_BUILD})
set_property(TARGET azgra PROPERTY CXX_STANDARD 17)

target_link_libraries(${PROJECT_NAME} stdc++fs)

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS locale)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
    message("LINKED BOOST")
endif ()

add_library(sub::azgra ALIAS ${PROJECT_NAME})
# add_library(sub::azgraS ALIAS ${PROJECT_NAME})

target_include_directories(azgra PUBLIC ${PROJECT_SOURCE_DIR}/include)
# target_include_directories(azgraS PUBLIC ${PROJECT_SOURCE_DIR}/include)

if (MATPLOTLIB_CPP)
    message("Configuring MATPLOTLIB_CPP")
    find_package(PythonLibs 2.7 REQUIRED)

    target_include_directories(azgra PUBLIC ${PYTHON_INCLUDE_DIRS})
    target_link_libraries(azgra ${PYTHON_LIBRARIES})

    # Define symbol for C++ code.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMATPLOTLIB_CPP")
endif ()

add_executable(consoleApp test.cpp)
target_link_libraries(consoleApp azgra)
set_property(TARGET consoleApp PROPERTY CXX_STANDARD 17)

if (AZGRA_TEST)
    #add_subdirectory(tests)

    add_executable(azgra-test tests/test.cpp)
    set_property(TARGET azgra-test PROPERTY CXX_STANDARD 17)

    find_package(Catch2 REQUIRED)
    target_link_libraries(azgra-test Catch2::Catch2)
    target_link_libraries(azgra-test azgra)

    include(CTest)
    include(Catch)
    catch_discover_tests(azgra-test)
endif()
