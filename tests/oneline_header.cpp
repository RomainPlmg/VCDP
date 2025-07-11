#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "vcdp/VCDP.hpp"

TEST_CASE("Open one line VCD header file") {
    vcdp::VCDParser parser;
    vcdp::VCDFile* trace = parser.Parse(TEST_DATA_DIR "oneline_header.vcd");
    REQUIRE(trace != nullptr);

    CHECK(trace->date == "Thu Jul 03 14:17:23 2025");
    CHECK(trace->version == "GHDL v0");
    CHECK(trace->time_resolution == 1);
    CHECK(trace->time_units == vcdp::VCDTimeUnit::TIME_NS);
    CHECK(trace->comment == "This is a test VCD file");
}