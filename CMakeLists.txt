cmake_minimum_required(VERSION 3.8...3.19)

project(VCDP VERSION 0.1.0 DESCRIPTION "A VCD parser inspired by ben-marshall/verilog-vcd-parser")

add_subdirectory(libs/pegtl)

add_library(vcdp INTERFACE)

target_link_libraries(vcdp INTERFACE pegtl)
target_include_directories(vcdp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)