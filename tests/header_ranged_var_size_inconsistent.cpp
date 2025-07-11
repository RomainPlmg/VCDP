#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "vcdp/VCDP.hpp"

TEST_CASE("Ranged var size in the header is inconsistent") {
    vcdp::VCDParser parser;
    auto trace = parser.Parse(TEST_DATA_DIR "header_ranged_var_size_inconsistent.vcd");

    REQUIRE(trace == nullptr);
    CHECK_FALSE(parser.GetResult().success);
    auto message = parser.GetResult().errors[0];
    CHECK(message.find("Range size mismatch for signal") != std::string::npos);
}