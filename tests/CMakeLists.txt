cmake_minimum_required(VERSION 3.8...3.19)

set(test_sources
        "empty_file.cpp"
        "header_incomplete.cpp"
        "header_simple.cpp"
        "header_comment.cpp"
        "header_empty_comment.cpp"
        "header_oneline.cpp"
        "header_tabed.cpp"
        "header_multiline_comment.cpp"
        "header_stuck_timescale.cpp"
        "header_simple_scope.cpp"
        "header_nested_scopes.cpp"
        "header_multi_topscopes.cpp"
        "header_missing_scope.cpp"
        "header_missing_upscope.cpp"
        "header_simple_var.cpp"
        "header_indexed_var.cpp"
        "header_ranged_var.cpp"
        "header_var_without_scope.cpp"
        "header_indexed_var_size_inconsistent.cpp"
        "header_ranged_var_size_inconsistent.cpp"
        "header_var_types.cpp"
        "big_file.cpp"
)

# file(GLOB ...) is used to validate the above list of test_sources
file(GLOB glob_test_sources RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.cpp)

foreach (test_source_file ${test_sources})
    if (${test_source_file} IN_LIST glob_test_sources)
        list(REMOVE_ITEM glob_test_sources ${test_source_file})
    else ()
        message(SEND_ERROR "File ${test_source_file} is missing from tests/")
    endif ()

    get_filename_component(exe_name vcdp-test-${test_source_file} NAME_WE)
    add_executable(${exe_name} ${test_source_file})
    target_link_libraries(${exe_name} PRIVATE vcdplib doctest)
    set_target_properties(${exe_name} PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
    )
    if (MSVC)
        target_compile_options(${exe_name} PRIVATE /W4 /WX /utf-8)
    else ()
        target_compile_options(${exe_name} PRIVATE -pedantic -Wall -Wextra -Wshadow -Werror)
    endif ()

    target_compile_definitions(${exe_name} PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data/")

    add_test(NAME ${exe_name} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ${exe_name})

endforeach ()

if (glob_test_sources)
    foreach (ignored_source_file ${glob_test_sources})
        message(SEND_ERROR "File ${ignored_source_file} in tests/ is ignored")
    endforeach ()
endif ()