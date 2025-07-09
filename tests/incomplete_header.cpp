#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "VCDP.hpp"

TEST_CASE("Open minimal VCD header file") {
    vcdp::VCDFile* trace = nullptr;
    REQUIRE(trace == nullptr);

    delete trace;
}