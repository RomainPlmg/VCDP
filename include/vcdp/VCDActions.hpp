#pragma once

#include <string>

#include "Config.hpp"
#include "VCDFile.hpp"
#include "VCDLexical.hpp"
#include "VCDValidationError.hpp"

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
        if (unit == "s")
            file.time_units = VCDTimeUnit::TIME_S;
        else if (unit == "ms")
            file.time_units = VCDTimeUnit::TIME_MS;
        else if (unit == "us")
            file.time_units = VCDTimeUnit::TIME_US;
        else if (unit == "ns")
            file.time_units = VCDTimeUnit::TIME_NS;
        else if (unit == "ps")
            file.time_units = VCDTimeUnit::TIME_PS;
        else if (unit == "fs")
            file.time_units = VCDTimeUnit::TIME_FS;
        else
            file.time_units = VCDTimeUnit::TIME_UNKNOWN;
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

/// @brief Capture scope type & store it into the scope builder
template <>
struct action<lexical::scope_type> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        std::string type = in.string();
        if (type == "begin")
            file.current_scope_builder.type = VCDScopeType::VCD_SCOPE_BEGIN;
        else if (type == "fork")
            file.current_scope_builder.type = VCDScopeType::VCD_SCOPE_FORK;
        else if (type == "function")
            file.current_scope_builder.type = VCDScopeType::VCD_SCOPE_FUNCTION;
        else if (type == "module")
            file.current_scope_builder.type = VCDScopeType::VCD_SCOPE_MODULE;
        else if (type == "task")
            file.current_scope_builder.type = VCDScopeType::VCD_SCOPE_TASK;
        else
            file.current_scope_builder.type = VCDScopeType::VCD_SCOPE_UNKNOWN;
    }
};

/// @brief Capture scope identifier, store it into the scope builder & build the new scope
template <>
struct action<lexical::scope_identifier> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.current_scope_builder.name = in.string();
        file.AddScope();
    }
};

/// @brief Reset the current scope to its parent
template <>
struct action<lexical::command_upscope> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        if (file.current_scope != nullptr)
            file.current_scope = file.current_scope->parent;
        else
            throw VCDValidationError("$scope declaration expected");
    }
};

/// @brief Capture variable type
template <>
struct action<lexical::var_type> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        std::string type = in.string();
        if (type == "event")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_EVENT;
        else if (type == "integer")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_INTEGER;
        else if (type == "parameter")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_PARAMETER;
        else if (type == "real")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_REAL;
        else if (type == "realtime")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_REALTIME;
        else if (type == "reg")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_REG;
        else if (type == "supply0")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_SUPPLY0;
        else if (type == "supply1")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_SUPPLY1;
        else if (type == "time")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_TIME;
        else if (type == "tri")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_TRI;
        else if (type == "triand")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_TRIAND;
        else if (type == "trior")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_TRIOR;
        else if (type == "trireg")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_TRIREG;
        else if (type == "tri0")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_TRI0;
        else if (type == "tri1")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_TRI1;
        else if (type == "wand")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_WAND;
        else if (type == "wire")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_WIRE;
        else if (type == "wor")
            file.current_signal_builder.type = VCDVarType::VCD_VAR_WOR;
        else
            file.current_signal_builder.type = VCDVarType::VCD_VAR_UNKNOWN;
    }
};

/// @brief Capture variable size
template <>
struct action<lexical::var_size> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.current_signal_builder.size = std::stoi(in.string());
    }
};

/// @brief Capture variable ID
template <>
struct action<lexical::var_identifier> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.current_signal_builder.hash = in.string();
    }
};

/// @brief Capture variable name
template <>
struct action<lexical::var_name> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.current_signal_builder.reference = in.string();
    }
};

/// @brief Capture lsb index
template <>
struct action<lexical::lsb_index> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.current_signal_builder.rindex = std::stoi(in.string());
    }
};

/// @brief Capture msb index
template <>
struct action<lexical::msb_index> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.current_signal_builder.lindex = std::stoi(in.string());
    }
};

/// @brief Capture single bit range signal
template <>
struct action<lexical::var_end> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.AddSignal();
    }
};

/// @brief Final header check
template <>
struct action<lexical::command_enddefinitions> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        if (file.current_scope != nullptr) throw VCDValidationError("$upscope declaration expected");
    }
};

}  // namespace VCDP_NAMESPACE
