#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "vcdp/VCDP.hpp"

TEST_CASE("Basic var section in the header") {
    vcdp::VCDParser parser;
    vcdp::VCDFile* trace = parser.Parse(TEST_DATA_DIR "header_simple_scope.vcd");
    REQUIRE(trace != nullptr);

    const auto& top_module = trace->GetScope("tb_counter");
    REQUIRE(top_module != nullptr);
    CHECK(top_module->signals.size() == 1);
    const auto& signal = top_module->signals.at(0);
    CHECK(signal->hash == "!");
    CHECK(signal->reference == "clk");
    CHECK(signal->scope == top_module);
    CHECK(signal->size == 1);
    CHECK(signal->type == vcdp::VCDVarType::VCD_VAR_WIRE);
    CHECK(signal->lindex == -1);
    CHECK(signal->rindex == -1);
}