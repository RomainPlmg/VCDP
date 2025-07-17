#include "vcdp/VCDValue.hpp"

#include <sstream>

#include "vcdp/Utils.hpp"

namespace VCDP_NAMESPACE {

VCDBit VCDValue::GetValBit() const {
    if (const auto* val = std::get_if<VCDBit>(&m_Data)) {
        return *val;
    }
    throw std::bad_variant_access();
}

const VCDBitVector& VCDValue::GetValVector() const {
    if (const auto* val = std::get_if<VCDBitVector>(&m_Data)) {
        return *val;
    }
    throw std::bad_variant_access();
}

VCDReal VCDValue::GetValReal() const {
    if (const auto* val = std::get_if<VCDReal>(&m_Data)) {
        return *val;
    }
    throw std::bad_variant_access();
}

std::string VCDValue::ToString() const {
    std::ostringstream oss;

    switch (m_Type) {
        case VCDValueType::VCD_SCALAR: {
            VCDBit bit = std::get<VCDBit>(m_Data);
            oss << utils::BitColor(bit) << utils::VCDBit2Char(bit) << color::RESET;
            break;
        }

        case VCDValueType::VCD_VECTOR: {
            const auto& vec = std::get<VCDBitVector>(m_Data);
            oss << "0b";
            for (VCDBit bit : vec) {
                oss << utils::BitColor(bit) << utils::VCDBit2Char(bit) << color::RESET;
            }
            break;
        }

        case VCDValueType::VCD_REAL: {
            VCDReal realVal = std::get<VCDReal>(m_Data);
            oss << realVal;
            break;
        }

        default:
            oss << color::RED << "<Invalid>" << color::RESET;
            break;
    }

    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const VCDValue& value) {
    os << value.ToString();

    return os;
}

}  // namespace VCDP_NAMESPACE