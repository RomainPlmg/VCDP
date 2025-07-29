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
    void addScope(std::unique_ptr<VCDScope> scope);

    /**
     * @brief Add a new signal to the VCD file.
     * @param signal The VCDSignal object to add to the VCD file.
     */
    void addSignal(std::unique_ptr<VCDSignal> signal);

    /**
     * @brief Add a new signal value on the last registered timestamp
     * @param delta The time between the last time and the new one
     * @param val The value of the signal
     * @param hash The VCD hash value representing the signal.
     */
    void addSignalValue(VCDTime delta, const VCDValue& val, const VCDSignalHash& hash);

    /**
     * @brief Return the scope object in the VCD file with this name.
     * @param name The name of the scope to get and return.
     * @return A pointer to the scope, or nullptr if scope not found.
     */
    [[nodiscard]] VCDScope* getScope(const VCDScopeName& name) const;

    /**
     * @brief Return the signal object in the VCD file with this symbol.
     * @param hash The symbol of the signal to get and return.
     * @return A pointer to the signal, or nullptr if signal not found.
     */
    [[nodiscard]] VCDSignal* getSignal(const VCDSignalHash& hash) const;

    /**
     * @brief Get the value of a particular signal at a specified time.
     * @note The supplied time value does not need to exist in the vector returned by GetTimestamps().
     * @param hash The hashcode for the signal to identify it.
     * @param time The time at which we want the value of the signal.
     * @returns A pointer to the value at the supplied time, or nullptr if no such record can be found.
     */
    [[nodiscard]] VCDValue getSignalValue(const VCDSignalHash& hash, VCDTime time);

    /**
     * @brief Get a vector of VCD time values
     * @param hash The hashcode for the signal to identify it.
     * @returns A pointer to the vector of time values, or nullptr if hash not found
     */
    [[nodiscard]] std::vector<VCDTime>& getSignalTimestamps(const VCDSignalHash& hash);

    /// @brief Get a vector of all scopes present in the file.
    [[nodiscard]] const std::vector<std::unique_ptr<VCDScope>>& getScopes() const { return scopes_; }

    /// @brief Return a flattened vector of all signals in the file.
    [[nodiscard]] const std::unordered_map<VCDSignalHash, std::unique_ptr<VCDSignal>>& getSignals() const { return signals_; }

    /// @brief Check if hash exists in value map
    [[nodiscard]] bool exists(const VCDSignalHash& hash) const;

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
    std::unordered_map<VCDSignalHash, std::unique_ptr<VCDSignal>> signals_;
    std::vector<std::unique_ptr<VCDScope>> scopes_;
};

}  // namespace VCDP_NAMESPACE
