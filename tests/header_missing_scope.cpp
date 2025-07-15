#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "vcdp/VCDP.hpp"

TEST_CASE("Missing $scope declaration in header") {
    vcdp::VCDParser parser;
    auto trace = parser.Parse(TEST_DATA_DIR "header_missing_scope.vcd");
    for (const auto& error : parser.GetResult().errors) {
        std::cerr << error << std::endl;
    }

    REQUIRE(trace == nullptr);
    CHECK_FALSE(parser.GetResult().success);
    auto message = parser.GetResult().errors[0];
    CHECK(message.find("$scope declaration expected") != std::string::npos);
}