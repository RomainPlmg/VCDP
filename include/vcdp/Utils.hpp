#pragma once

#include "Config.hpp"
#include "VCDTypes.hpp"

namespace VCDP_NAMESPACE::utils {

// clang-format off
std::string VCDVarType2String(VCDVarType type) {
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
// clang-format on

}  // namespace VCDP_NAMESPACE::utils

namespace VCDP_NAMESPACE::color {

inline constexpr const char* RESET = "\033[0m";

inline constexpr const char* BLACK = "\033[30m";
inline constexpr const char* RED = "\033[31m";
inline constexpr const char* GREEN = "\033[32m";
inline constexpr const char* YELLOW = "\033[33m";
inline constexpr const char* BLUE = "\033[34m";
inline constexpr const char* MAGENTA = "\033[35m";
inline constexpr const char* CYAN = "\033[36m";
inline constexpr const char* WHITE = "\033[37m";

inline constexpr const char* BRIGHT_BLACK = "\033[90m";
inline constexpr const char* BRIGHT_RED = "\033[91m";
inline constexpr const char* BRIGHT_GREEN = "\033[92m";
inline constexpr const char* BRIGHT_YELLOW = "\033[93m";
inline constexpr const char* BRIGHT_BLUE = "\033[94m";
inline constexpr const char* BRIGHT_MAGENTA = "\033[95m";
inline constexpr const char* BRIGHT_CYAN = "\033[96m";
inline constexpr const char* BRIGHT_WHITE = "\033[97m";

inline constexpr const char* BG_RED = "\033[41m";
inline constexpr const char* BG_GREEN = "\033[42m";
inline constexpr const char* BG_YELLOW = "\033[43m";
inline constexpr const char* BG_BLUE = "\033[44m";
inline constexpr const char* BG_MAGENTA = "\033[45m";
inline constexpr const char* BG_CYAN = "\033[46m";
inline constexpr const char* BG_WHITE = "\033[47m";

constexpr const char* BOLD_BLACK   = "\033[1;30m";
constexpr const char* BOLD_RED     = "\033[1;31m";
constexpr const char* BOLD_GREEN   = "\033[1;32m";
constexpr const char* BOLD_YELLOW  = "\033[1;33m";
constexpr const char* BOLD_BLUE    = "\033[1;34m";
constexpr const char* BOLD_MAGENTA = "\033[1;35m";
constexpr const char* BOLD_CYAN    = "\033[1;36m";
constexpr const char* BOLD_WHITE   = "\033[1;37m";

}  // namespace VCDP_NAMESPACE::color
