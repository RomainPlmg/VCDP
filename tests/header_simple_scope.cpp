#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "vcdp/VCDP.hpp"

TEST_CASE("Basic scope section in the header") {
    vcdp::VCDParser parser;
    vcdp::VCDFile* trace = parser.Parse(TEST_DATA_DIR "header_simple_scope.vcd");
    REQUIRE(trace != nullptr);

    CHECK(trace->date == "Tue Jul  8 15:36:41 2025");
    CHECK(trace->version == "QuestaSim Version 2024.2");
    CHECK(trace->time_resolution == 1);
    CHECK(trace->time_units == vcdp::VCDTimeUnit::TIME_NS);
    CHECK(trace->GetScopes().size() == 1);

    const auto& top_module = trace->GetScope("tb_counter");
    CHECK(top_module != nullptr);
    CHECK(top_module->name == "tb_counter");
    CHECK(top_module->type == vcdp::VCDScopeType::VCD_SCOPE_MODULE);
}