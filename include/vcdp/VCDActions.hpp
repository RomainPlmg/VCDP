#pragma once

#include <string>

#include "Config.hpp"
#include "Utils.hpp"
#include "VCDFile.hpp"
#include "VCDLexical.hpp"
#include "VCDValue.hpp"

namespace VCDP_NAMESPACE {

struct VCDScopeBuilder {
    VCDScopeName name;
    VCDScopeType type = VCDScopeType::VCD_SCOPE_UNKNOWN;

    [[nodiscard]] bool IsComplete() const { return !name.empty() && type != VCDScopeType::VCD_SCOPE_UNKNOWN; }

    void Reset() {
        name.clear();
        type = VCDScopeType::VCD_SCOPE_UNKNOWN;
    }

    std::unique_ptr<VCDScope> Build(VCDScope* parent) {
        auto scope = std::make_unique<VCDScope>();
        scope->name = name;
        scope->type = type;
        scope->parent = parent;

        Reset();

        return std::move(scope);
    }
};

struct VCDSignalBuilder {
    VCDSignalHash hash;
    VCDSignalReference reference;
    VCDSignalSize size = 0;
    VCDVarType type = VCDVarType::VCD_VAR_UNKNOWN;
    int rindex = -1;
    int lindex = -1;

    [[nodiscard]] bool IsComplete() const { return !hash.empty() && !reference.empty() && size > 0 && type != VCDVarType::VCD_VAR_UNKNOWN; }

    void Reset() {
        hash.clear();
        reference.clear();
        size = 0;
        type = VCDVarType::VCD_VAR_UNKNOWN;
        rindex = -1;
        lindex = -1;
    }

    std::unique_ptr<VCDSignal> Build(VCDScope* scope) {
        auto signal = std::make_unique<VCDSignal>();
        signal->reference = reference;
        signal->type = type;
        signal->scope = scope;
        signal->hash = hash;
        signal->rindex = rindex;
        signal->lindex = lindex;
        signal->size = size;

        Reset();

        return std::move(signal);
    }
};

struct ActionState {
    VCDScopeBuilder current_scope_builder;
    VCDSignalBuilder current_signal_builder;
};

template <typename Rule>
struct action : pegtl::nothing<Rule> {};

template <>
struct action<lexical::text_date> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file, ActionState& state) {
        file.date = in.string();
    }
};

template <>
struct action<lexical::time_number> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file, ActionState& state) {
        file.time_resolution = std::stoi(in.string());
    }
};

template <>
struct action<lexical::time_unit> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file, ActionState& state) {
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

template <>
struct action<lexical::text_version> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file, ActionState& state) {
        file.version = in.string();
    }
};

template <>
struct action<lexical::scope_type> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file, ActionState& state) {
        std::string type = in.string();
        if (type == "begin")
            state.current_scope_builder.type = VCDScopeType::VCD_SCOPE_BEGIN;
        else if (type == "fork")
            state.current_scope_builder.type = VCDScopeType::VCD_SCOPE_FORK;
        else if (type == "function")
            state.current_scope_builder.type = VCDScopeType::VCD_SCOPE_FUNCTION;
        else if (type == "module")
            state.current_scope_builder.type = VCDScopeType::VCD_SCOPE_MODULE;
        else if (type == "task")
            state.current_scope_builder.type = VCDScopeType::VCD_SCOPE_TASK;
        else
            state.current_scope_builder.type = VCDScopeType::VCD_SCOPE_UNKNOWN;
    }
};

template <>
struct action<lexical::scope_identifier> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file, ActionState& state) {
        state.current_scope_builder.name = in.string();
    }
};

template <>
struct action<lexical::scope_end> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file, ActionState& state) {
        if (state.current_scope_builder.IsComplete()) {
            auto scope = state.current_scope_builder.Build(file.current_scope);
            file.addScope(std::move(scope));
        }
    }
};

template <>
struct action<lexical::command_upscope> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file, ActionState& state) {
        if (file.current_scope != nullptr) {
            file.current_scope = file.current_scope->parent;
        } else {
            throw pegtl::parse_error("$scope declaration expected", in);
        }
    }
};

template <>
struct action<lexical::var_type> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file, ActionState& state) {
        std::string type = in.string();
        if (type == "event")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_EVENT;
        else if (type == "integer")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_INTEGER;
        else if (type == "parameter")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_PARAMETER;
        else if (type == "real")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_REAL;
        else if (type == "realtime")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_REALTIME;
        else if (type == "reg")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_REG;
        else if (type == "supply0")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_SUPPLY0;
        else if (type == "supply1")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_SUPPLY1;
        else if (type == "time")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_TIME;
        else if (type == "tri")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_TRI;
        else if (type == "triand")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_TRIAND;
        else if (type == "trior")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_TRIOR;
        else if (type == "trireg")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_TRIREG;
        else if (type == "tri0")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_TRI0;
        else if (type == "tri1")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_TRI1;
        else if (type == "wand")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_WAND;
        else if (type == "wire")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_WIRE;
        else if (type == "wor")
            state.current_signal_builder.type = VCDVarType::VCD_VAR_WOR;
        else
            state.current_signal_builder.type = VCDVarType::VCD_VAR_UNKNOWN;
    }
};

template <>
struct action<lexical::var_size> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file, ActionState& state) {
        const uint32_t size = std::stoi(in.string());
        state.current_signal_builder.size = size;
    }
};

template <>
struct action<lexical::var_identifier> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file, ActionState& state) {
        const std::string hash = in.string();
        state.current_signal_builder.hash = hash;
    }
};

template <>
struct action<lexical::var_name> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file, ActionState& state) {
        state.current_signal_builder.reference = in.string();
    }
};

template <>
struct action<lexical::lsb_index> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file, ActionState& state) {
        state.current_signal_builder.rindex = std::stoi(in.string());
    }
};

template <>
struct action<lexical::msb_index> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file, ActionState& state) {
        state.current_signal_builder.lindex = std::stoi(in.string());
    }
};

template <>
struct action<lexical::var_end> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file, ActionState& state) {
        if (state.current_signal_builder.IsComplete()) {
            auto signal = state.current_signal_builder.Build(file.current_scope);
            if (signal->scope == nullptr) {
                std::ostringstream msg;
                msg << "Signal \'" << signal->reference << "\' needs to be part of a scope";
                throw pegtl::parse_error(msg.str(), in);
            }

            if (signal->rindex != -1) {
                if (signal->lindex == -1 && signal->size != -1) {
                    std::ostringstream msg;
                    msg << "Size mismatch for signal '" << signal->reference << "': index [" << signal->rindex
                        << "] implies size 1 but declared size is " << signal->size;
                    throw pegtl::parse_error(msg.str(), in);
                }
                if (signal->size > 1) {
                    // Range size exception
                    if (int range_size = std::abs(signal->lindex - signal->rindex) + 1; range_size != signal->size) {
                        std::ostringstream msg;
                        msg << "Range size mismatch for signal '" << signal->reference << "': range [" << signal->lindex << ":" << signal->rindex
                            << "] implies size " << range_size << " but declared size is " << signal->size;
                        throw pegtl::parse_error(msg.str(), in);
                    }
                } else {
                    // Single index exception
                    if (signal->size != 1) {
                        std::ostringstream msg;
                        msg << "Size mismatch for signal '" << signal->reference << "': index [" << signal->rindex
                            << "] implies size 1 but declared size is " << signal->size;
                        throw pegtl::parse_error(msg.str(), in);
                    }
                }
            }
            file.addSignal(std::move(signal));
        }
    }
};

template <>
struct action<lexical::command_enddefinitions> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file, ActionState& state) {
        if (file.current_scope != nullptr) {
            throw pegtl::parse_error("$upscope declaration expected.", in);
        }
    }
};

}  // namespace VCDP_NAMESPACE
