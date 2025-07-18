#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "vcdp/VCDP.hpp"

TEST_CASE("Nested scopes in the header") {
    vcdp::VCDParser parser;
    auto trace = parser.Parse(TEST_DATA_DIR "header_nested_scopes.vcd");
    for (const auto& error : parser.GetResult().errors) {
        std::cerr << error << std::endl;
    }
    REQUIRE(trace != nullptr);

    CHECK(trace->GetScopes().size() == 3);

    // Top module
    const auto& top_module = trace->GetScope("tb_counter");
    REQUIRE(top_module != nullptr);
    CHECK(top_module->name == "tb_counter");
    CHECK(top_module->type == vcdp::VCDScopeType::VCD_SCOPE_MODULE);
    CHECK(top_module->parent == nullptr);
    CHECK(top_module->children.size() == 1);

    // Sub module
    const auto& sub_module = top_module->children.at(0);
    REQUIRE(sub_module != nullptr);
    CHECK(sub_module->name == "uut");
    CHECK(sub_module->type == vcdp::VCDScopeType::VCD_SCOPE_MODULE);
    CHECK(sub_module->parent == top_module);
    CHECK(sub_module->children.size() == 1);

    // Sub-sub module
    const auto& sub2_module = sub_module->children.at(0);
    REQUIRE(sub2_module != nullptr);
    CHECK(sub2_module->name == "and");
    CHECK(sub2_module->type == vcdp::VCDScopeType::VCD_SCOPE_MODULE);
    CHECK(sub2_module->parent == sub_module);
    CHECK(sub2_module->children.size() == 0);

    // Nested module
}