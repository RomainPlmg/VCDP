#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "vcdp/VCDP.hpp"

TEST_CASE("Var out of scope in the header") {
    vcdp::VCDParser parser;
    auto trace = parser.Parse(TEST_DATA_DIR "header_var_without_scope.vcd");

    REQUIRE(trace == nullptr);
    CHECK_FALSE(parser.GetResult().success);
    auto message = parser.GetResult().errors[0];
    CHECK(message.find("needs to be part of a scope") != std::string::npos);
}