cmake_minimum_required(VERSION 3.8...3.19)

project(VCDP VERSION 0.1.0 DESCRIPTION "A VCD parser inspired by ben-marshall/verilog-vcd-parser" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Désactiver clang-tidy avec MSVC
    set(CMAKE_CXX_CLANG_TIDY "")
    message(STATUS "Clang-Tidy disabled with MSVC")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Activer seulement avec Clang
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
endif()

enable_testing()

add_subdirectory(libs/pegtl)
add_subdirectory(libs/doctest)

file(GLOB_RECURSE SRC_FILES
        src/*.hpp
        src/*.cpp
)

option(BUILD_STATIC_LIB "Build as a static library instead of header-only" OFF)

add_library(vcdp STATIC ${SRC_FILES})

target_link_libraries(vcdp PUBLIC pegtl)
target_include_directories(vcdp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(tests)