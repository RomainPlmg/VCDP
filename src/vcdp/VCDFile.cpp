#include "vcdp/VCDFile.hpp"

namespace VCDP_NAMESPACE {

VCDFile::~VCDFile() {
    // Delete signals & scopes.
    for (const auto& scope : m_Scopes) {
        for (const auto& signal : scope->signals) {
            delete signal;
        }
    }

    // Delete signal values.
    for (auto& [hash, values] : m_ValMap) {
        for (const auto& value : *values) {
            delete value->value;
            delete value;
        }
        delete values;
    }
}

void VCDFile::AddScope() {
    if (current_scope_builder.IsComplete()) {
        auto scope = current_scope_builder.Build(current_scope);
        VCDScope* child_scope = scope.get();
        m_Scopes.push_back(std::move(scope));
        if (current_scope != nullptr) current_scope->children.push_back(child_scope);
        current_scope = child_scope;
        current_scope_builder = VCDScopeBuilder{};  // Reset
    }
}

void VCDFile::AddSignal(VCDSignal* p_signal) {
    if (current_signal_builder.IsComplete()) {
        auto signal = current_signal_builder.Build(current_scope);
        m_Signals.push_back(std::move(signal));

        // Add timestamp entry
        if (m_ValMap.find(p_signal->hash) == m_ValMap.end()) {
            // Value will be populated later
            m_ValMap[p_signal->hash] = new VCDSignalValues();
        }

        current_signal_builder = VCDSignalBuilder{};  // Reset
    }
}

VCDScope* VCDFile::GetScope(const VCDScopeName& name) const {
    for (const auto& scope : m_Scopes) {
        if (scope->name == name) return scope.get();
    }
    return nullptr;
}

VCDValue* VCDFile::GetSignalValue(const VCDSignalHash& hash, const VCDTime time, bool erase_prior) {
    const auto it = m_ValMap.find(hash);
    if (it == m_ValMap.end()) return nullptr;

    const auto& values = it->second;
    auto erase_until = values->begin();
    if (values->empty()) return nullptr;

    VCDValue* rt = nullptr;

    for (auto value_it = values->begin(); value_it != values->end(); ++value_it) {
        if ((*value_it)->time <= time) {
            erase_until = value_it;
            rt = (*value_it)->value;
        } else {
            break;
        }
    }

    if (erase_prior) {
        // Avoid O(n^2) performance for large sequential scans
        for (auto i = values->begin(); i != erase_until; ++i) {
            delete (*i)->value;
        }
        values->erase(values->begin(), erase_until);
    }

    return rt;
}

VCDSignalValues* VCDFile::GetSignalValues(const VCDSignalHash& hash) {
    if (m_ValMap.find(hash) != m_ValMap.end()) return m_ValMap.at(hash);

    return nullptr;
}

}  // namespace VCDP_NAMESPACE