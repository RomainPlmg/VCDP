#include "VCDValue.h"

namespace VCDP_NAMESPACE {

char VCDValue::VCDBit2Char(const VCDBit bit) {
    switch (bit) {
        case VCDBit::VCD_0:
            return '0';
        case VCDBit::VCD_1:
            return '1';
        case VCDBit::VCD_Z:
            return 'Z';
        case VCDBit::VCD_X:
            return 'X';
        default:
            return '?';
    }
}

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