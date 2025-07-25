#pragma once

#include <memory>
#include <unordered_map>

#include "Config.hpp"
#include "VCDTypes.hpp"
#include "VCDValue.hpp"

/// @brief Top level object to represent a single VCD file.

namespace VCDP_NAMESPACE {

class VCDFile {
   public:
    /// @brief Instance a new VCD file container.
    VCDFile() = default;

    /**
     * @brief Add a new scope object to the VCD file.
     * @param scope The VCDScope object to add to the VCD file.
     */
    void AddScope(std::unique_ptr<VCDScope>& scope);

    /**
     * @brief Add a new signal to the VCD file.
     * @param signal The VCDSignal object to add to the VCD file.
     */
    void AddSignal(std::unique_ptr<VCDSignal>& signal);

    /**
     * @brief Add a new timestamp value to the VCD file.
     * @details Add a time stamp to the sorted array of existing timestamps in the file.
     * @param time The timestamp value to add to the file.
     */
    void AddTimestamp(VCDTime time);

    /**
     * @brief Add a new signal value to the VCD file, tagged by time.
     * @param time_val A signal value, tagged by the time it occurs.
     * @param hash The VCD hash value representing the signal.
     */
    void AddSignalValue(VCDTimedValue time_val, const VCDSignalHash& hash);

    /**
     * @brief Return the scope object in the VCD file with this name.
     * @param name The name of the scope to get and return.
     * @return A pointer to the scope, or nullptr if scope not found.
     */
    [[nodiscard]] VCDScope* GetScope(const VCDScopeName& name) const;

    /**
     * @brief Return the signal object in the VCD file with this symbol.
     * @param hash The symbol of the signal to get and return.
     * @return A pointer to the signal, or nullptr if signal not found.
     */
    [[nodiscard]] VCDSignal* GetSignal(const VCDSignalHash& hash) const;

    /**
     * @brief Get the value of a particular signal at a specified time.
     * @note The supplied time value does not need to exist in the vector returned by GetTimestamps().
     * @param hash The hashcode for the signal to identify it.
     * @param time The time at which we want the value of the signal.
     * @param erase_prior Erase signals prior to this time. Avoids O(n^2) searching times when scanning large .vcd files sequentially.
     * @returns A pointer to the value at the supplied time, or nullptr if no such record can be found.
     */
    [[nodiscard]] VCDValue* GetSignalValue(const VCDSignalHash& hash, VCDTime time, bool erase_prior = false);

    /**
     * @brief Get a vector of VCD time values
     * @param hash The hashcode for the signal to identify it.
     * @returns A pointer to the vector of time values, or nullptr if hash not found
     */
    [[nodiscard]] VCDSignalValues* GetSignalValues(const VCDSignalHash& hash);

    /// @brief Return a reference to the set of timestamp samples present in the VCD file.
    [[nodiscard]] const std::vector<VCDTime>& GetTimestamps() const { return m_Times; }

    /// @brief Get a vector of all scopes present in the file.
    [[nodiscard]] const std::vector<std::unique_ptr<VCDScope>>& GetScopes() const { return m_Scopes; }

    /// @brief Return a flattened vector of all signals in the file.
    [[nodiscard]] const std::vector<std::unique_ptr<VCDSignal>>& GetSignals() const { return m_Signals; }

    /// @brief Check if hash exists in value map
    [[nodiscard]] bool Exists(const VCDSignalHash& hash) const { return m_ValMap.contains(hash); }

    /// @brief Timescale of the VCD file.
    VCDTimeUnit time_units = VCDTimeUnit::TIME_UNKNOWN;

    /// @brief Multiplier of the VCD file time units.
    VCDTimeRes time_resolution = 0;

    /// @brief Date string of the VCD file
    std::string date;

    /// @brief Version string of the simulator which generated the VCD.
    std::string version;

    /// @brief Current scope nodes of the VCD signals -> To manage parents & children
    VCDScope* current_scope = nullptr;

   private:
    std::vector<std::unique_ptr<VCDSignal>> m_Signals;
    std::vector<std::unique_ptr<VCDScope>> m_Scopes;
    std::vector<VCDTime> m_Times;
    std::unordered_map<VCDSignalHash, VCDSignalValues> m_ValMap;
};

}  // namespace VCDP_NAMESPACE
