#pragma once

#include "Config.hpp"

namespace VCDP_NAMESPACE {

class VCDValidationError : public std::runtime_error {
   public:
    VCDValidationError(const std::string& msg) : std::runtime_error(msg) {}
};

}  // namespace VCDP_NAMESPACE
