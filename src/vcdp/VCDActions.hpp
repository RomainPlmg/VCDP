#pragma once

#include "Config.hpp"
#include "VCDFile.hpp"
#include "VCDLexical.hpp"

namespace VCDP_NAMESPACE {

inline std::string NormalizeSpaces(const std::string& input) {
    std::istringstream iss(input);
    std::ostringstream oss;
    std::string word;

    bool first = true;
    while (iss >> word) {
        if (!first) {
            oss << " ";
        }
        oss << word;
        first = false;
    }

    return oss.str();
}

template <typename Rule>
struct action : pegtl::nothing<Rule> {};

/// @brief Capture comment & store it into the VCD file container
template <>
struct action<lexical::text_comment> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        const std::string comment = in.string();

        file.comment = NormalizeSpaces(comment);
    }
};

/// @brief Capture date & store it into the VCD file container
template <>
struct action<lexical::text_date> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.date = in.string();
    }
};

/// @brief Capture time resolution & store it into the VCD file container
template <>
struct action<lexical::time_number> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.time_resolution = std::stoi(in.string());
    }
};

/// @brief Capture time unit & store it into the VCD file container
template <>
struct action<lexical::time_unit> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        std::string unit = in.string();
        if (unit == "s") file.time_units = VCDTimeUnit::TIME_S;
        else if (unit == "ms") file.time_units = VCDTimeUnit::TIME_MS;
        else if (unit == "us") file.time_units = VCDTimeUnit::TIME_US;
        else if (unit == "ns") file.time_units = VCDTimeUnit::TIME_NS;
        else if (unit == "ps") file.time_units = VCDTimeUnit::TIME_PS;
        else if (unit == "fs") file.time_units = VCDTimeUnit::TIME_FS;
        else file.time_units = VCDTimeUnit::TIME_UNKNOWN;
    }
};

/// @brief Capture version & store it into the VCD file container
template <>
struct action<lexical::text_version> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.version = in.string();
    }
};

}  // namespace VCDP_NAMESPACE
