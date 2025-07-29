#pragma once

#include <algorithm>
#include <sstream>

#include "Config.hpp"
#include "Utils.hpp"
#include "VCDTypes.hpp"

namespace VCDP_NAMESPACE {

/*!
 * @brief Represents a single value found in a VCD File.
 * @details Can contain a single bit (a scalar), a bit vector, or an IEEE floating point number.
 */
struct VCDValue {
    VCDValueType type : 2;
    size_t vector_size = 0;

    union {
        VCDBit scalar;
        VCDBit* vector;
        VCDReal real;
    };

    /// @brief Constructor
    VCDValue() : type(VCDValueType::VCD_SCALAR), scalar(VCDBit::VCD_0) {}
    /// @brief for scalar
    explicit VCDValue(const VCDBit bit) : type(VCDValueType::VCD_SCALAR), scalar(bit) {}
    /// @brief For large vector (> 64 bits)
    explicit VCDValue(const VCDBit* bits, const size_t size) : type(VCDValueType::VCD_VECTOR) {
        if (!bits || size == 0) {
            vector_size = 0;
            vector = nullptr;
            return;
        }
        vector_size = size;
        vector = new VCDBit[size];
        std::copy(bits, bits + size, vector);
    }

    /// @brief For real
    explicit VCDValue(const VCDReal r) : type(VCDValueType::VCD_REAL), real(r) {}

    /// @brief Destructor
    ~VCDValue() {
        if (type == VCDValueType::VCD_VECTOR) {
            delete[] vector;
            vector = nullptr;
        }
    }

    /// @brief Copy constructor
    VCDValue(const VCDValue& other) : type(other.type), vector_size(other.vector_size) {
        switch (type) {
            case VCDValueType::VCD_SCALAR:
                scalar = other.scalar;
                break;
            case VCDValueType::VCD_VECTOR:
                if (vector_size) {
                    vector = new VCDBit[vector_size];
                    std::copy_n(other.vector, vector_size, vector);
                } else {
                    vector = nullptr;
                }
                break;
            case VCDValueType::VCD_REAL:
                real = other.real;
                break;
            default:
                break;
        }
    }

    [[nodiscard]] std::string ToString() const {
        std::ostringstream oss;
        switch (type) {
            case VCDValueType::VCD_SCALAR:
                oss << utils::vcdBit2Char(scalar);
                return oss.str();
            case VCDValueType::VCD_VECTOR: {
                oss << "0b";
                for (size_t i = 0; i < vector_size; i++) {
                    oss << utils::vcdBit2Char(vector[i]);
                }
                return oss.str();
            }
            case VCDValueType::VCD_REAL:
                oss << real;

            default:
                oss << "?";
                break;
        }

        return oss.str();
    };

    friend std::ostream& operator<<(std::ostream& os, const VCDValue& value) { return os << value.ToString(); }
};

/// @brief A signal value tagged with times.
struct VCDSignalChange {
    VCDTime delta;
    VCDValue value;
};

using VCDSignalChanges = std::vector<VCDSignalChange>;

}  // namespace VCDP_NAMESPACE
