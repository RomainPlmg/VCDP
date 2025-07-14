#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "vcdp/VCDP.hpp"

TEST_CASE("Indexed var size in the header is inconsistent") {
    vcdp::VCDParser parser;
    auto trace = parser.Parse(TEST_DATA_DIR "header_indexed_var_size_inconsistent.vcd");
    for (const auto& error : parser.GetResult().errors) {
        std::cerr << error << std::endl;
    }

    REQUIRE(trace == nullptr);
    CHECK_FALSE(parser.GetResult().success);
    auto message = parser.GetResult().errors[0];
    CHECK(message.find("Size mismatch for signal") != std::string::npos);
}