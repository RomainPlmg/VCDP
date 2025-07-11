#pragma once

#include <sstream>

#include "Config.hpp"
#include "VCDTypes.hpp"
#include "VCDValidationError.hpp"

namespace VCDP_NAMESPACE::checkers {

static void ValidateVCDSignal(const VCDSignal& signal) {
    if (signal.scope == nullptr) {
        std::ostringstream oss;
        oss << "Signal \'" << signal.reference << "\' needs to be part of a scope";
        throw VCDValidationError(oss.str());
    }

    if (signal.rindex != -1) {
        if (signal.lindex != -1) {
            // Range size exception
            int range_size = std::abs(signal.lindex - signal.rindex) + 1;
            if (range_size != signal.size) {
                std::ostringstream oss;
                oss << "Range size mismatch for signal '" << signal.reference << "': range [" << signal.lindex << ":" << signal.rindex
                    << "] implies size " << range_size << " but declared size is " << signal.size;
                throw VCDValidationError(oss.str());
            }
        } else {
            // Single index exception
            if (signal.size != 1) {
                std::ostringstream oss;
                oss << "Size mismatch for signal '" << signal.reference << "': index [" << signal.rindex << "] implies size 1 but declared size is "
                    << signal.size;
                throw VCDValidationError(oss.str());
            }
        }
    }
}

}  // namespace VCDP_NAMESPACE::checkers
