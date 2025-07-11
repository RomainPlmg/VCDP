#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "vcdp/VCDP.hpp"

TEST_CASE("Comment section is multiple lines") {
    vcdp::VCDParser parser;
    vcdp::VCDFile* trace = parser.Parse(TEST_DATA_DIR "header_multiline_comment.vcd");
    REQUIRE(trace != nullptr);

    CHECK(trace->comment == "This is a test VCD file with multiple lines comment");
}