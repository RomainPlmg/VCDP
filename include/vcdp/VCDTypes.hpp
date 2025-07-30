#pragma once

#include <string>
#include <vector>

#include "Config.hpp"
#include "VListManager.hpp"

/**
 * @file VCDTypes.hpp
 * @brief A file for common types and data structures used by the VCD parser.
 */

namespace VCDP_NAMESPACE {

/// @brief Describes how a signal value is represented in the VCD trace.
enum class VCDValueType : unsigned char {
    VCD_SCALAR,  //!< Single VCDBit
    VCD_VECTOR,  //!< Vector of VCDBit
    VCD_REAL     //!< IEEE Floating point (64bit).
};

enum class VCDBit : uint8_t { VCD_0 = 0, VCD_X = 1, VCD_1 = 2, VCD_Z = 3, VCD_H = 5, VCD_U = 7, VCD_W = 9, VCD_L = 11, VCD_DC = 13, VCD_UNK = 15 };

/// @brief Variable types of a signal in a VCD file.
enum class VCDVarType {
    VCD_VAR_UNKNOWN,
    VCD_VAR_EVENT,
    VCD_VAR_INTEGER,
    VCD_VAR_PARAMETER,
    VCD_VAR_REAL,
    VCD_VAR_REALTIME,
    VCD_VAR_REG,
    VCD_VAR_SUPPLY0,
    VCD_VAR_SUPPLY1,
    VCD_VAR_TIME,
    VCD_VAR_TRI,
    VCD_VAR_TRIAND,
    VCD_VAR_TRIOR,
    VCD_VAR_TRIREG,
    VCD_VAR_TRI0,
    VCD_VAR_TRI1,
    VCD_VAR_WAND,
    VCD_VAR_WIRE,
    VCD_VAR_WOR
};

/// @brief Represents the possible time units a VCD file is specified in.
enum class VCDTimeUnit {
    TIME_UNKNOWN,  ///< Unknown
    TIME_S,        ///< Seconds
    TIME_MS,       ///< Milliseconds
    TIME_US,       ///< Microseconds
    TIME_NS,       ///< Nanoseconds
    TIME_PS,       ///< Picoseconds
    TIME_FS,       ///< Femtoseconde
};

/// @brief Represents the type of SV construct whose scope we are in.
enum class VCDScopeType { VCD_SCOPE_UNKNOWN, VCD_SCOPE_BEGIN, VCD_SCOPE_FORK, VCD_SCOPE_FUNCTION, VCD_SCOPE_MODULE, VCD_SCOPE_TASK, VCD_SCOPE_ROOT };

// Forward declaration of VCDScope to make it available to VCDSignal struct.
struct VCDScope;

/// @brief Represents a single signal reference within a VCD file
struct VCDSignal {
    std::string hash;
    std::string reference;
    VCDScope* scope = nullptr;
    uint32_t size = 0;
    VCDVarType type = VCDVarType::VCD_VAR_UNKNOWN;
    int lindex = -1;  // -1 if no brackets, otherwise [lindex] or [lindex:rindex]
    int rindex = -1;  // -1 if not [lindex:rindex]

    VListManager data;
};

/// @brief Represents a scope type, scope name pair and all of its child signals.
struct VCDScope {
    std::string name;                 //!< The short name of the scope
    VCDScopeType type;                //!< Construct type
    VCDScope* parent;                 //!< Parent scope object
    std::vector<VCDScope*> children;  //!< Child scope objects.
    std::vector<VCDSignal*> signals;  //!< Signals in this scope.
};
}  // namespace VCDP_NAMESPACE