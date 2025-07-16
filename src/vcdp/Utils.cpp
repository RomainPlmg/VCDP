#include "vcdp/Utils.hpp"

namespace VCDP_NAMESPACE::utils {

// clang-format off
std::string VCDVarType2String(VCDVarType type)  {
    switch (type) {
        case VCDVarType::VCD_VAR_UNKNOWN:   return "unknown";
        case VCDVarType::VCD_VAR_EVENT:     return "event";
        case VCDVarType::VCD_VAR_INTEGER:   return "integer";
        case VCDVarType::VCD_VAR_PARAMETER: return "parameter";
        case VCDVarType::VCD_VAR_REAL:      return "real";
        case VCDVarType::VCD_VAR_REALTIME:  return "realtime";
        case VCDVarType::VCD_VAR_REG:       return "reg";
        case VCDVarType::VCD_VAR_SUPPLY0:   return "supply0";
        case VCDVarType::VCD_VAR_SUPPLY1:   return "supply1";
        case VCDVarType::VCD_VAR_TIME:      return "time";
        case VCDVarType::VCD_VAR_TRI:       return "tri";
        case VCDVarType::VCD_VAR_TRIAND:    return "triand";
        case VCDVarType::VCD_VAR_TRIOR:     return "trior";
        case VCDVarType::VCD_VAR_TRIREG:    return "trireg";
        case VCDVarType::VCD_VAR_TRI0:      return "tri0";
        case VCDVarType::VCD_VAR_TRI1:      return "tri1";
        case VCDVarType::VCD_VAR_WAND:      return "wand";
        case VCDVarType::VCD_VAR_WIRE:      return "wire";
        case VCDVarType::VCD_VAR_WOR:       return "wor";
        default:                            return "invalid";
    }
}
std::string VCDTimeUnit2String(VCDTimeUnit unit) {
    switch (unit) {
    case VCDTimeUnit::TIME_UNKNOWN: return "unknown";
    case VCDTimeUnit::TIME_S:       return "s";
    case VCDTimeUnit::TIME_MS:      return "ms";
    case VCDTimeUnit::TIME_US:      return "us";
    case VCDTimeUnit::TIME_NS:      return "ns";
    case VCDTimeUnit::TIME_PS:      return "ps";
    case VCDTimeUnit::TIME_FS:      return "fs";
    default:                        return "invalid";
    }
}
// clang-format on

}  // namespace VCDP_NAMESPACE::utils