#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "vcdp/VCDP.hpp"

TEST_CASE("Basic scope section in the header") {
    vcdp::VCDParser parser;
    vcdp::VCDFile* trace = parser.Parse(TEST_DATA_DIR "header_simple_scope.vcd");
    REQUIRE(trace != nullptr);

    CHECK(trace->GetScopes().size() == 1);
    const auto& top_module = trace->GetScope("tb_counter");
    REQUIRE(top_module != nullptr);
    CHECK(top_module->name == "tb_counter");
    CHECK(top_module->type == vcdp::VCDScopeType::VCD_SCOPE_MODULE);
}