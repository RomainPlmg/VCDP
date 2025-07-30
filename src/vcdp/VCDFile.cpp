#include "vcdp/VCDFile.hpp"

#include <iostream>
#include <ranges>
#include <sstream>

#include "vcdp/VCDTypes.hpp"

namespace VCDP_NAMESPACE {

void VCDFile::addScope(std::unique_ptr<VCDScope> scope) {
    scopes_.push_back(std::move(scope));
    VCDScope* p_scope = scopes_.back().get();

    if (current_scope != nullptr) current_scope->children.push_back(p_scope);
    current_scope = p_scope;
}

void VCDFile::addSignal(std::unique_ptr<VCDSignal> signal) {
    const std::string hash = signal->hash;

    if (!signals_.contains(signal->hash)) {
        signals_[hash] = std::move(signal);
    }

    VCDSignal* p_signal = signals_[hash].get();
    current_scope->signals.push_back(p_signal);
}

void VCDFile::addTimestamp(const uint64_t timestamp) { times_.push_back(timestamp); }

VCDScope* VCDFile::getScope(const std::string& name) const {
    for (const auto& scope : scopes_) {
        if (scope->name == name) return scope.get();
    }
    return nullptr;
}

VCDSignal* VCDFile::getSignal(const std::string& hash) const {
    for (const auto& signal : signals_ | std::views::values) {
        if (signal->hash == hash) {
            return signal.get();
        }
    }
    return nullptr;
}

uint64_t VCDFile::getTimestamp(const size_t index) const {
    if (index >= times_.size()) return 0;
    return times_[index];
}

const std::vector<uint64_t>& VCDFile::getTimestamps() const { return times_; }

bool VCDFile::exists(const std::string& hash) const {
    if (const auto it = signals_.find(hash); it == signals_.end()) {
        std::ostringstream msg;
        msg << "Unable to find the signal hash \'" << hash << "\'";
        std::cerr << msg.str() << std::endl;
        return false;
    }
    return true;
}

}  // namespace VCDP_NAMESPACE