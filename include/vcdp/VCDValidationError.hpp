#pragma once

#include "Config.hpp"
#include "VCDLexical.hpp"

namespace VCDP_NAMESPACE {

class VCDValidationError : public std::runtime_error {
   public:
    VCDValidationError(const std::string& msg) : std::runtime_error(msg) {}
};

template <typename>
inline constexpr const char* error_message = nullptr;

// template <>
// inline constexpr auto error_message<lexical::dkw_comment> = "Invalid $comment keyword";
// template <>
// inline constexpr auto error_message<lexical::dkw_date> = "Invalid $date keyword";
// template <>
// inline constexpr auto error_message<lexical::dkw_enddefinitions> = "Invalid $enddefinitions keyword";
// template <>
// inline constexpr auto error_message<lexical::dkw_scope> = "Invalid $scope keyword";
// template <>
// inline constexpr auto error_message<lexical::dkw_timescale> = "Invalid $timescale keyword";
// template <>
// inline constexpr auto error_message<lexical::dkw_upscope> = "Invalid $upscope keyword";
// template <>
// inline constexpr auto error_message<lexical::dkw_var> = "Invalid $var keyword";

struct error {
    template <typename Rule>
    static constexpr auto message = error_message<Rule>;
};

template <typename Rule>
using control = pegtl::must_if<error>::control<Rule>;

}  // namespace VCDP_NAMESPACE
