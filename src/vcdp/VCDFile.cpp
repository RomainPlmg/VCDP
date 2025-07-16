#include "vcdp/VCDFile.hpp"

#include "vcdp/VCDFile.hpp"
#include "vcdp/VCDTypes.hpp"

namespace VCDP_NAMESPACE {

VCDFile::~VCDFile() {
    // Delete signal values.
    for (auto& [hash, values] : m_ValMap) {
        for (const auto& value : *values) {
            delete value->value;
            delete value;
        }
        delete values;
    }
}

void VCDFile::AddScope(std::unique_ptr<VCDScope>& scope) {
        VCDScope* p_scope = scope.get();
        m_Scopes.push_back(std::move(scope));
        if (current_scope != nullptr) current_scope->children.push_back(p_scope);
        current_scope = p_scope;
}

void VCDFile::AddSignal(std::unique_ptr<VCDSignal>& signal) {
        VCDSignal* p_signal = signal.get();
        m_Signals.push_back(std::move(signal));
        current_scope->signals.push_back(p_signal);

        // Add timestamp entry
        if (m_ValMap.find(p_signal->hash) == m_ValMap.end()) {
            // Value will be populated later
            m_ValMap[p_signal->hash] = new VCDSignalValues();
        }
}

void VCDFile::AddTimestamp(const VCDTime time) { m_Times.push_back(time); }

VCDScope* VCDFile::GetScope(const VCDScopeName& name) const {
    for (const auto& scope : m_Scopes) {
        if (scope->name == name) return scope.get();
    }
    return nullptr;
}

void VCDFile::AddSignalValue(VCDTimedValue* p_time_val, const VCDSignalHash& hash) { m_ValMap[hash]->push_back(p_time_val); }

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