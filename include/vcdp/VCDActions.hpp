#pragma once

#include <string>

#include "Config.hpp"
#include "Utils.hpp"
#include "VCDFile.hpp"
#include "VCDLexical.hpp"

namespace VCDP_NAMESPACE {

static bool in_header = true;
static VCDTime current_time = 0;
static bool time_init = false;
static VCDValueType current_value_type = VCDValueType::VCD_SCALAR;
static VCDBit current_scalar = VCDBit::VCD_X;
static VCDBitVector current_bit_vector;
static VCDReal current_real = 0.0;

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

template <>
struct action<lexical::text_comment> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        const std::string comment = in.string();

        file.comment = NormalizeSpaces(comment);
    }
};

template <>
struct action<lexical::text_date> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.date = in.string();
    }
};

template <>
struct action<lexical::time_number> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.time_resolution = std::stoi(in.string());
    }
};

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

template <>
struct action<lexical::text_version> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.version = in.string();
    }
};

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

template <>
struct action<lexical::scope_identifier> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.current_scope_builder.name = in.string();
    }
};

template <>
struct action<lexical::scope_end> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        if (file.current_scope_builder.IsComplete()) {
            auto scope = file.current_scope_builder.Build(file.current_scope);
            file.AddScope(scope);
        }
    }
};

template <>
struct action<lexical::command_upscope> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
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

template <>
struct action<lexical::var_size> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.current_signal_builder.size = std::stoi(in.string());
    }
};

template <>
struct action<lexical::var_identifier> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        std::string hash = in.string();
        if (in_header) {
            file.current_signal_builder.hash = hash;
        } else {
            // If signal doesn't exists
            if (file.GetSignalValues(hash) == nullptr) {
                std::ostringstream msg;
                msg << "Unknown hash \'" << hash << "\'.";
                throw pegtl::parse_error(msg.str(), in);
            }

            // Register value change
            VCDTimedValue timed_value = {};
            if (current_value_type == VCDValueType::VCD_SCALAR) {
                timed_value = {current_time, new VCDValue(current_scalar)};
            } else if (current_value_type == VCDValueType::VCD_VECTOR) {
                timed_value = {current_time, new VCDValue(current_bit_vector)};
            } else {
                timed_value = {current_time, new VCDValue(current_real)};
            }

            file.AddSignalValue(new VCDTimedValue(timed_value), hash);
        }
    }
};

template <>
struct action<lexical::var_name> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.current_signal_builder.reference = in.string();
    }
};

template <>
struct action<lexical::lsb_index> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.current_signal_builder.rindex = std::stoi(in.string());
    }
};

template <>
struct action<lexical::msb_index> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        file.current_signal_builder.lindex = std::stoi(in.string());
    }
};

template <>
struct action<lexical::var_end> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        if (file.current_signal_builder.IsComplete()) {
            auto signal = file.current_signal_builder.Build(file.current_scope);
            if (signal->scope == nullptr) {
                std::ostringstream msg;
                msg << "Signal \'" << signal->reference << "\' needs to be part of a scope";
                throw pegtl::parse_error(msg.str(), in);
            }

            if (signal->rindex != -1) {
                if (signal->lindex != -1) {
                    // Range size exception
                    int range_size = std::abs(signal->lindex - signal->rindex) + 1;
                    if (range_size != signal->size) {
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
            file.AddSignal(signal);
        }
    }
};

template <>
struct action<lexical::command_enddefinitions> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        if (file.current_scope != nullptr) {
            throw pegtl::parse_error("$upscope declaration expected.", in);
        }
        in_header = false;
    }
};

template <>
struct action<lexical::skw_dumpvars> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        if (!time_init) {
            time_init = true;
            file.AddTimestamp(current_time);
        }
    }
};

template <>
struct action<lexical::scalar_value> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        if (!time_init) {
            throw pegtl::parse_error("Timestamp not initialized. Use \'#0\' or \'$dump...\' keywords", in);
        }

        VCDBit bit = utils::Char2VCDBit(in.string()[0]);
        if (current_value_type == VCDValueType::VCD_SCALAR) {
            current_scalar = bit;
        } else if (current_value_type == VCDValueType::VCD_VECTOR) {
            current_bit_vector.push_back(bit);
        }
    }
};

template <>
struct action<lexical::real_number> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        if (!time_init) {
            throw pegtl::parse_error("Timestamp not initialized. Use \'#0\' or \'$dump...\' keywords", in);
        }

        if (current_value_type == VCDValueType::VCD_REAL) {
            current_real = std::stod(in.string());
        } else {
            throw pegtl::parse_error("Real number not expected.", in);
        }
    }
};

template <>
struct action<lexical::binary_vector_prefix> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        current_value_type = VCDValueType::VCD_VECTOR;
        current_bit_vector.clear();
    }
};

template <>
struct action<lexical::real_vector_prefix> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        current_value_type = VCDValueType::VCD_REAL;
    }
};

template <>
struct action<lexical::vector_value_change> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        current_value_type = VCDValueType::VCD_SCALAR;
    }
};

template <>
struct action<lexical::timestamp_number> {
    template <typename Input>
    static void apply(const Input& in, VCDFile& file) {
        uint64_t time = std::stoull(in.string());
        if (time == 0) {
            time_init = true;
        }

        if (time < current_time) {
            std::ostringstream msg;
            msg << "The current timestamp cannot be less than the previous one. New: " << time << vcdp::utils::VCDTimeUnit2String(file.time_units)
                << " | Previous: " << current_time << vcdp::utils::VCDTimeUnit2String(file.time_units);
            throw pegtl::parse_error(msg.str(), in);
        }
        current_time = time;
        file.AddTimestamp(current_time);
    }
};

}  // namespace VCDP_NAMESPACE
