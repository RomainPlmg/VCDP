#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "vcdp/VCDP.hpp"

TEST_CASE("Indexed var section in the header") {
    vcdp::VCDParser parser;
    auto trace = parser.Parse(TEST_DATA_DIR "header_indexed_var.vcd");
    for (const auto& error : parser.GetResult().errors) {
        std::cerr << error << std::endl;
    }
    REQUIRE(trace != nullptr);

    const auto& top_module = trace->GetScope("tb_counter");
    REQUIRE(top_module != nullptr);
    CHECK(top_module->signals.size() == 1);
    const auto& signal = top_module->signals.at(0);
    REQUIRE(signal != nullptr);
    CHECK(signal->hash == "!");
    CHECK(signal->reference == "data");
    CHECK(signal->scope == top_module);
    CHECK(signal->size == 1);
    CHECK(signal->type == vcdp::VCDVarType::VCD_VAR_WIRE);
    CHECK(signal->lindex == -1);
    CHECK(signal->rindex == 0);
}