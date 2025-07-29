#include "vcdp/VCDFile.hpp"

#include <ranges>
#include <sstream>

#include "vcdp/VCDTypes.hpp"
#include "vcdp/VCDValue.hpp"

namespace VCDP_NAMESPACE {

void VCDFile::addScope(std::unique_ptr<VCDScope> scope) {
    scopes_.push_back(std::move(scope));
    VCDScope* p_scope = scopes_.back().get();

    if (current_scope != nullptr) current_scope->children.push_back(p_scope);
    current_scope = p_scope;
}

void VCDFile::addSignal(std::unique_ptr<VCDSignal> signal) {
    const VCDSignalHash hash = signal->hash;

    if (!signals_.contains(signal->hash)) {
        signals_[hash] = std::move(signal);
    }

    VCDSignal* p_signal = signals_[hash].get();
    current_scope->signals.push_back(p_signal);
}

VCDScope* VCDFile::getScope(const VCDScopeName& name) const {
    for (const auto& scope : scopes_) {
        if (scope->name == name) return scope.get();
    }
    return nullptr;
}

VCDSignal* VCDFile::getSignal(const VCDSignalHash& hash) const {
    for (const auto& signal : signals_ | std::views::values) {
        if (signal->hash == hash) {
            return signal.get();
        }
    }
    return nullptr;
}

void VCDFile::addSignalValue(const VCDTime delta, const VCDValue& val, const VCDSignalHash& hash) {
    if (const auto& it = signals_.find(hash); it == signals_.end()) {
        std::ostringstream msg;
        msg << "Unable to find the signal hash \'" << hash << "\'";
        throw std::runtime_error(msg.str());
    }

    const auto& signal = signals_.at(hash).get();
    signal->timestamps.push_back(delta);
    switch (val.type) {
        case VCDValueType::VCD_SCALAR:
            signal->bits.push_back(val.scalar);
            break;
        case VCDValueType::VCD_VECTOR: {
            auto* bits = new VCDBit[val.vector_size];
            std::copy_n(val.vector, val.vector_size, bits);
            signal->vectors.push_back(bits);
            break;
        }
        case VCDValueType::VCD_REAL:
            signal->reals.push_back(val.real);
            break;
        default:
            break;
    }
}

VCDValue VCDFile::getSignalValue(const VCDSignalHash& hash, const VCDTime time) {
    const auto it = signals_.find(hash);
    if (it == signals_.end()) {
        std::ostringstream msg;
        msg << "Unable to find the signal hash \'" << hash << "\'";
        throw std::runtime_error(msg.str());
    }
    const auto& signal = it->second;

    if (signal->timestamps.empty()) {
        return {};
    }

    size_t index = 0;

    for (const auto& timestamp : signal->timestamps) {
        if (timestamp >= time) {
            break;
        }
        index++;
    }

    if (!signal->bits.empty()) {
        return VCDValue(signal->bits.at(index));
    }
    if (!signal->vectors.empty()) {
        return VCDValue(signal->vectors.at(index), signal->size);
    }
    if (!signal->reals.empty()) {
        return VCDValue(signal->reals.at(index));
    }

    return {};
}

std::vector<VCDTime>& VCDFile::getSignalTimestamps(const VCDSignalHash& hash) {
    const auto it = signals_.find(hash);
    if (it == signals_.end()) {
        std::ostringstream msg;
        msg << "Unable to find the signal hash \'" << hash << "\'";
        throw std::runtime_error(msg.str());
    }
    const auto& signal = it->second;
    return signal->timestamps;
}

bool VCDFile::exists(const VCDSignalHash& hash) const {
    if (const auto it = signals_.find(hash); it == signals_.end()) {
        std::ostringstream msg;
        msg << "Unable to find the signal hash \'" << hash << "\'";
        return false;
    }
    return true;
}

}  // namespace VCDP_NAMESPACE