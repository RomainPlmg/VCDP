#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "vcdp/VCDP.hpp"

TEST_CASE("Basic header with a comment section") {
    vcdp::VCDParser parser;
    auto trace = parser.Parse(TEST_DATA_DIR "header_comment.vcd");
    for (const auto& error : parser.GetResult().errors) {
        std::cerr << error << std::endl;
    }
    REQUIRE(trace != nullptr);
}