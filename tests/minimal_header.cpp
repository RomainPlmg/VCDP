#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "VCDP.hpp"

TEST_CASE("Open minimal VCD header file") {
    vcdp::VCDFile* trace = new vcdp::VCDFile();
    REQUIRE(trace != nullptr);

    CHECK(trace->date == "Thu Jul 03 14:17:23 2025");
    CHECK(trace->version == "GHDL v0");
    CHECK(trace->time_resolution == 1);
    CHECK(trace->time_units == vcdp::VCDTimeUnit::TIME_NS);

    delete trace;
}
