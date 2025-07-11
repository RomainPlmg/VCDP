#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "vcdp/VCDP.hpp"

TEST_CASE("Multi top scopes in the header") {
    vcdp::VCDParser parser;
    vcdp::VCDFile* trace = parser.Parse(TEST_DATA_DIR "header_multi_topscopes.vcd");
    REQUIRE(trace != nullptr);

    CHECK(trace->date == "Tue Jul  8 15:36:41 2025");
    CHECK(trace->version == "QuestaSim Version 2024.2");
    CHECK(trace->time_resolution == 1);
    CHECK(trace->time_units == vcdp::VCDTimeUnit::TIME_NS);

    CHECK(trace->GetScopes().size() == 3);

    // Top module 1
    const auto& top_module = trace->GetScope("tb_counter");
    REQUIRE(top_module != nullptr);
    CHECK(top_module->name == "tb_counter");
    CHECK(top_module->type == vcdp::VCDScopeType::VCD_SCOPE_MODULE);
    CHECK(top_module->parent == nullptr);
    CHECK(top_module->children.size() == 0);

    // Top module 2
    const auto& top2_module = trace->GetScope("uut");
    REQUIRE(top2_module != nullptr);
    CHECK(top2_module->name == "uut");
    CHECK(top2_module->type == vcdp::VCDScopeType::VCD_SCOPE_MODULE);
    CHECK(top2_module->parent == nullptr);
    CHECK(top2_module->children.size() == 0);

    // Top module 3
    const auto& top3_module = trace->GetScope("and");
    REQUIRE(top3_module != nullptr);
    CHECK(top3_module->name == "and");
    CHECK(top3_module->type == vcdp::VCDScopeType::VCD_SCOPE_MODULE);
    CHECK(top3_module->parent == nullptr);
    CHECK(top3_module->children.size() == 0);

    // Nested module
}