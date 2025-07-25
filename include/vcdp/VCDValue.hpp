#pragma once

#include <iostream>

#include "Config.hpp"
#include "VCDTypes.hpp"

namespace VCDP_NAMESPACE {

/*!
 * @brief Represents a single value found in a VCD File.
 * @details Can contain a single bit (a scalar), a bit vector, or an IEEE floating point number.
 */

class VCDValue {
   public:
    /// @brief Create a new VCDValue with the type VCD_SCALAR
    explicit VCDValue(VCDBit data) : m_Type(VCDValueType::VCD_SCALAR), m_Data(data) {}

    /// @brief Create a new VCDValue with the type VCD_VECTOR
    explicit VCDValue(VCDBitVector& data) : m_Type(VCDValueType::VCD_VECTOR), m_Data(data) {}

    /// @brief Create a new VCDValue with the type VCD_REAL
    explicit VCDValue(VCDReal data) : m_Type(VCDValueType::VCD_REAL), m_Data(data) {}

    /// @brief Destroy VCD value container
    ~VCDValue() = default;

    /// @brief Return the type of value stored by this class instance.
    [[nodiscard]] VCDValueType GetType() const { return m_Type; }

    /// @brief Get the bit value of the instance.
    [[nodiscard]] VCDBit GetValBit() const;

    /// @brief Get the vector value of the instance.
    [[nodiscard]] const VCDBitVector& GetValVector() const;

    /// @brief Get the real value of the instance.
    [[nodiscard]] VCDReal GetValReal() const;

    /// @brief Print the value
    [[nodiscard]] std::string ToString() const;

    friend std::ostream& operator<<(std::ostream& os, const VCDValue& value);

   private:
    VCDValueType m_Type;
    VCDValueVariant m_Data;
};

/// @brief A signal value tagged with times.
struct VCDTimedValue {
    VCDTime time;
    std::unique_ptr<VCDValue> value;
};

/// @brief A vector of tagged time/value pairs, sorted by time values.
using VCDSignalValues = std::vector<VCDTimedValue>;

}  // namespace VCDP_NAMESPACE
