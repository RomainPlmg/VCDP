#pragma once

#include <unordered_map>

#include "Config.hpp"
#include "VCDTypes.hpp"

/**
 * @brief Top level object to represent a single VCD file.
 */

namespace VCDP_NAMESPACE {

class VCDFile {
   public:
    /// @brief Instance a new VCD file container.
    VCDFile() = default;

    /// @brief Destroy VCD file container.
    ~VCDFile() {
        // Delete signals & scopes.
        for (auto& scope : m_Scopes) {
            for (auto& signal : scope->signals) {
                delete signal;
            }
            delete scope;
        }

        // Delete signal values.
        auto hash_val = m_ValMap.begin();
        for (hash_val; hash_val != m_ValMap.end(); hash_val++) {
            auto vals = hash_val->second->begin();
            for (vals; vals != hash_val->second->end(); vals++) {
                delete (*vals)->value;
                delete *vals;
            }
            delete hash_val->second;
        }
    }

    /**
     * @brief Add a new scope object to the VCD file.
     * @param p_scope The VCDScope object to add to the VCD file.
     */
    void AddScope(VCDScope* p_scope) { m_Scopes.push_back(p_scope); }

    /**
     * @brief Add a new signal to the VCD file.
     * @param p_signal The VCDSignal object to add to the VCD file.
     */
    void AddSignal(VCDSignal* p_signal) {
        m_Signals.push_back(p_signal);

        // Add timestamp entry
        if (m_ValMap.find(p_signal->hash) == m_ValMap.end()) {
            // Value will be populated later
            m_ValMap[p_signal->hash] = new VCDSignalValues();
        }
    }

    /**
     * @brief Add a new timestamp value to the VCD file.
     * @details Add a time stamp to the sorted array of existing timestamps in the file.
     * @param time The timestamp value to add to the file.
     */
    void AddTimestamp(VCDTime time) { m_Times.push_back(time); }

    /**
     * @brief Add a new signal value to the VCD file, tagged by time.
     * @param p_time_val A signal value, tagged by the time it occurs.
     * @param hash The VCD hash value representing the signal.
     */
    void AddSignalValue(VCDTimedValue* p_time_val, const VCDSignalHash& hash) { m_ValMap[hash]->push_back(p_time_val); }

    /**
     * @brief Return the scope object in the VCD file with this name.
     * @param name The name of the scope to get and return.
     * @return A pointer to the scope, or nullptr if scope not found.
     */
    VCDScope* GetScope(const VCDScopeName& name) {}

    /**
     * @brief Get the value of a particular signal at a specified time.
     * @note The supplied time value does not need to exist in the vector returned by GetTimestamps().
     * @param hash The hashcode for the signal to identify it.
     * @param time The time at which we want the value of the signal.
     * @param erase_prior Erase signals prior to this time. Avoids O(n^2) searching times when scanning large .vcd files sequentially.
     * @returns A pointer to the value at the supplie time, or nullptr if no such record can be found.
     */
    VCDValue* GetSignalValue(const VCDSignalHash& hash, VCDTime time, bool erase_prior = false) {}

    /**
     * @brief Get a vector of VCD time values
     * @param hash The hashcode for the signal to identify it.
     * @returns A pointer to the vector of time values, or nullptr if hash not found
     */
    VCDSignalValues* GetSignalValues(const VCDSignalHash& hash) {}

    /**
     * @brief Return a reference to the set of timestamp samples present in the VCD file.
     */
    std::vector<VCDTime>& GetTimestamps() {}

    /**
     * @brief Get a vector of all scopes present in the file.
     */
    std::vector<VCDScope*>& GetScopes() {}

    /**
     * @brief Return a flattened vector of all signals in the file.
     */
    std::vector<VCDSignal*>& GetSignals() {}

    /// @brief Timescale of the VCD file.
    VCDTimeUnit time_units;

    /// @brief Multiplier of the VCD file time units.
    VCDTimeRes time_resolution;

    /// @brief Date string of the VCD file
    std::string date;

    /// @brief Version string of the simulator which generated the VCD.
    std::string version;

    /// @brief Version string of the simulator which generated the VCD.
    std::string comment;

    /// @brief Root scope node of the VCD signals
    VCDScope* root_scope;

   private:
    std::vector<VCDSignal*> m_Signals;
    std::vector<VCDScope*> m_Scopes;
    std::vector<VCDTime> m_Times;
    std::unordered_map<VCDSignalHash, VCDSignalValues*> m_ValMap;
};

}  // namespace VCDP_NAMESPACE
