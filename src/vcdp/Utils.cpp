#include "vcdp/Utils.hpp"

namespace VCDP_NAMESPACE::utils {

// clang-format off
std::string vcdVarType2String(const VCDVarType type)  {
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
std::string vcdTimeUnit2String(const VCDTimeUnit unit) {
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

char vcdBit2Char(const VCDBit bit) {
    switch (bit) {
        case VCDBit::VCD_0      :   return '0';
        case VCDBit::VCD_1      :   return '1';
        case VCDBit::VCD_Z      :   return 'Z';
        case VCDBit::VCD_X      :   return 'X';
        case VCDBit::VCD_U      :   return 'U';
        case VCDBit::VCD_W      :   return 'W';
        case VCDBit::VCD_L      :   return 'L';
        case VCDBit::VCD_H      :   return 'H';
        case VCDBit::VCD_DC     :   return '-';
        case VCDBit::VCD_UNK    :   return '?';
        default:                    return '?';
    }
}

VCDBit char2VCDBit(const char bit) {
    switch (bit) {
        case '0':   return VCDBit::VCD_0;
        case '1':   return VCDBit::VCD_1;
        case 'x':   return VCDBit::VCD_X;
        case 'X':   return VCDBit::VCD_X;
        case 'z':   return VCDBit::VCD_Z;
        case 'Z':   return VCDBit::VCD_Z;
        case 'u':   return VCDBit::VCD_U;
        case 'U':   return VCDBit::VCD_U;
        case 'w':   return VCDBit::VCD_W;
        case 'W':   return VCDBit::VCD_W;
        case 'l':   return VCDBit::VCD_L;
        case 'L':   return VCDBit::VCD_L;
        case 'h':   return VCDBit::VCD_H;
        case 'H':   return VCDBit::VCD_H;
        case '-':   return VCDBit::VCD_DC;
        default:    return VCDBit::VCD_UNK;
    }
}

const char* bitColor(const VCDBit bit) {
    switch (bit) {
        case VCDBit::VCD_0  :   return color::GREEN;
        case VCDBit::VCD_1  :   return color::CYAN;
        case VCDBit::VCD_Z  :   return color::BLUE;
        case VCDBit::VCD_X  :   return color::RED;
        case VCDBit::VCD_U  :   return color::MAGENTA;
        case VCDBit::VCD_W  :   return color::BG_RED;
        case VCDBit::VCD_L  :   return color::BG_GREEN;
        case VCDBit::VCD_H  :   return color::BG_GREEN;
        case VCDBit::VCD_DC :   return color::WHITE;
        case VCDBit::VCD_UNK:   return color::RED;
        default:                return color::RED;
    }
}
// clang-format on

}  // namespace VCDP_NAMESPACE::utils