#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "vcdp/VCDP.hpp"

const u_int8_t VAR_NB = 17;
constexpr std::array<const vcdp::VCDVarType, VAR_NB> signal_types = {
    vcdp::VCDVarType::VCD_VAR_EVENT,    vcdp::VCDVarType::VCD_VAR_INTEGER, vcdp::VCDVarType::VCD_VAR_PARAMETER, vcdp::VCDVarType::VCD_VAR_REAL,
    vcdp::VCDVarType::VCD_VAR_REALTIME, vcdp::VCDVarType::VCD_VAR_REG,     vcdp::VCDVarType::VCD_VAR_SUPPLY0,   vcdp::VCDVarType::VCD_VAR_SUPPLY1,
    vcdp::VCDVarType::VCD_VAR_TIME,     vcdp::VCDVarType::VCD_VAR_TRI,     vcdp::VCDVarType::VCD_VAR_TRIAND,    vcdp::VCDVarType::VCD_VAR_TRIOR,
    vcdp::VCDVarType::VCD_VAR_TRIREG,   vcdp::VCDVarType::VCD_VAR_TRI0,    vcdp::VCDVarType::VCD_VAR_WAND,      vcdp::VCDVarType::VCD_VAR_WIRE,
    vcdp::VCDVarType::VCD_VAR_WOR};
const std::array<const vcdp::VCDSignalReference, VAR_NB> signal_name = {
    "sig_event", "sig_integer", "sig_parameter", "sig_real",   "sig_realtime", "sig_reg",  "sig_supply0", "sig_supply1", "sig_time",
    "sig_tri",   "sig_triand",  "sig_trior",     "sig_trireg", "sig_tri0",     "sig_wand", "sig_wire",    "sig_wor"};

TEST_CASE("Var type test in the header") {
    vcdp::VCDParser parser;
    vcdp::VCDFile* trace = parser.Parse(TEST_DATA_DIR "header_var_types.vcd");
    for (const auto& error : parser.GetResult().errors) {
        std::cerr << error << std::endl;
    }
    REQUIRE(trace != nullptr);

    const auto& top_module = trace->GetScope("test_var_types");
    REQUIRE(top_module != nullptr);
    CHECK(top_module->signals.size() == VAR_NB);

    for (size_t i = 0; i < VAR_NB; i++) {
        auto signal = top_module->signals.at(i);
        REQUIRE(signal != nullptr);
        CHECK(signal->type == signal_types.at(i));
        CHECK(signal->reference == signal_name.at(i));
    }
}