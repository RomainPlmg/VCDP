#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "Config.hpp"

/**
 * @file VCDTypes.hpp
 * @brief A file for common types and data structures used by the VCD parser.
 */

namespace VCDP_NAMESPACE {

/// @brief Friendly name for a signal
using VCDSignalReference = std::string;

/// @brief Friendly name for a scope
using VCDScopeName = std::string;

/// @brief Compressed hash representation of a signal.
using VCDSignalHash = std::string;

/// @brief Represents a single instant in time in a trace
using VCDTime = double;

/// @brief Specifies the timing resolution along with VCDTimeUnit
using VCDTimeRes = uint32_t;

/// @brief Width in bits of a signal.
using VCDSignalSize = uint32_t;

/// @brief Represents the four-state signal values of a VCD file.
enum class VCDBit {
    VCD_0 = 0,  //!< Logic zero
    VCD_1 = 1,  //!< Logic one
    VCD_X = 2,  //!< Unknown / Undefined
    VCD_Z = 3   //!< High Impedance.
};

/// @brief A vector of VCDBit values.
using VCDBitVector = std::vector<VCDBit>;

/// @brief Using to identify a real number as stored in a VCD.
using VCDReal = double;

/// @brief Variant type for a signal's value: bit, vector, or real.
using VCDValueVariant = std::variant<VCDBit, VCDBitVector, VCDReal>;

/// @brief Describes how a signal value is represented in the VCD trace.
enum class VCDValueType {
    VCD_SCALAR,  //!< Single VCDBit
    VCD_VECTOR,  //!< Vector of VCDBit
    VCD_REAL     //!< IEEE Floating point (64bit).
};

// Forward declaration of class.
class VCDValue;

/// @brief A signal value tagged with times.
struct VCDTimedValue {
    VCDTime time;
    VCDValue* value;
};

/// @brief A vector of tagged time/value pairs, sorted by time values.
using VCDSignalValues = std::vector<VCDTimedValue*>;

/// @brief Variable types of a signal in a VCD file.
enum class VCDVarType {
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
};

/// @brief Represents the type of SV construct whose scope we are in.
enum class VCDScopeType { VCD_SCOPE_BEGIN, VCD_SCOPE_FORK, VCD_SCOPE_FUNCTION, VCD_SCOPE_MODULE, VCD_SCOPE_TASK, VCD_SCOPE_ROOT };

// Forward declaration of VCDScope to make it available to VCDSignal struct.
struct VCDScope;

/// @brief Represents a single signal reference within a VCD file
struct VCDSignal {
    VCDSignalHash hash;
    VCDSignalReference reference;
    VCDScope* scope;
    VCDSignalSize size;
    VCDVarType type;
    int lindex;  // -1 if no brackets, otherwise [lindex] or [lindex:rindex]
    int rindex;  // -1 if not [lindex:rindex]
};

/// @brief Represents a scope type, scope name pair and all of its child signals.
struct VCDScope {
    VCDScopeName name;                //!< The short name of the scope
    VCDScopeType type;                //!< Construct type
    VCDScope* parent;                 //!< Parent scope object
    std::vector<VCDScope*> children;  //!< Child scope objects.
    std::vector<VCDSignal*> signals;  //!< Signals in this scope.
};

}  // namespace VCDP_NAMESPACE