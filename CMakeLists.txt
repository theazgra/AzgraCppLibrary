cmake_minimum_required(VERSION 3.0.0)

project(azgra)


#set(CMAKE_CXX_FLAGS "-Wall -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC")


set(OBJECTS_TO_BUILD src/stream/binary_converter.cpp
        src/stream/in_binary_stream_base.cpp
        src/stream/in_binary_file_stream.cpp
        src/stream/out_binary_stream_base.cpp
        src/stream/out_binary_file_stream.cpp
        src/stream/out_binary_buffer_stream.cpp
        src/stream/in_binary_buffer_stream.cpp
        src/stream/memory_bit_stream.cpp
        src/utilities/Stopwatch.cpp
        src/utilities/z_order.cpp
        src/string/simple_string.cpp
        src/utilities/guid.cpp
        src/fs/file_system.cpp
        src/cli/cli_arguments.cpp
        include/azgra/utilities/histogram.h
        include/azgra/string/smart_string_view.h
        include/azgra/utilities/print_helpers.h
        include/azgra/collection/set_utilities.h
        include/azgra/collection/vector_linq.h
        include/azgra/collection/vector_utilities.h
        include/azgra/io/text_file_functions.h
        include/azgra/matrix.h
        include/azgra/collection/linq.h
        )

add_library(azgra STATIC ${OBJECTS_TO_BUILD})
set_property(TARGET azgra PROPERTY CXX_STANDARD 17)

# add_library(azgraS SHARED ${OBJECTS_TO_BUILD})

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS filesystem locale)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
    message("LINKED BOOST")
endif ()

add_library(sub::azgra ALIAS ${PROJECT_NAME})
# add_library(sub::azgraS ALIAS ${PROJECT_NAME})

target_include_directories(azgra PUBLIC ${PROJECT_SOURCE_DIR}/include)
# target_include_directories(azgraS PUBLIC ${PROJECT_SOURCE_DIR}/include)
