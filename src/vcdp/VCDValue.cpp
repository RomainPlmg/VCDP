#include "vcdp/VCDValue.hpp"

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

}  // namespace VCDP_NAMESPACE