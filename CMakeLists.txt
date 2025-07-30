cmake_minimum_required(VERSION 3.8...3.19)

project(VCDP VERSION 0.1.0 DESCRIPTION "A VCD parser inspired by ben-marshall/verilog-vcd-parser" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(COMPARE EQUAL "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}" VCDP_STANDALONE)
option(VCDP_BUILD_TESTS "Build the VCDP test programs" ${VCDP_STANDALONE})

find_package(libdeflate REQUIRED)

if (VCDP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(libs/doctest)
    add_subdirectory(tests)
endif ()

add_subdirectory(libs/pegtl)
add_subdirectory(libs/argparse)

file(GLOB_RECURSE SRC_FILES
        src/*.hpp
        src/vcdp/*.cpp
)

option(BUILD_STATIC_LIB "Build as a static library instead of header-only" OFF)

add_library(vcdplib STATIC ${SRC_FILES})

target_link_libraries(vcdplib PUBLIC pegtl)
target_include_directories(vcdplib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Creating standalone executable
add_executable(vcdp src/main.cpp)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(vcdp PRIVATE -fexec-charset=UTF-8)
endif ()
target_link_libraries(vcdp PUBLIC vcdplib argparse)