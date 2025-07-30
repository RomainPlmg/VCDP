#pragma once

#include <map>
#include <memory>

#include "Config.hpp"
#include "VCDTypes.hpp"

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
     * @brief Add a new timestamp to the VCD file.
     * @param timestamp The timestamp value (eg. 123000).
     */
    void addTimestamp(uint64_t timestamp);

    /**
     * @brief Return the scope object in the VCD file with this name.
     * @param name The name of the scope to get and return.
     * @return A pointer to the scope, or nullptr if scope not found.
     */
    [[nodiscard]] VCDScope* getScope(const std::string& name) const;

    /**
     * @brief Return the signal object in the VCD file with this symbol.
     * @param hash The symbol of the signal to get and return.
     * @return A pointer to the signal, or nullptr if signal not found.
     */
    [[nodiscard]] VCDSignal* getSignal(const std::string& hash) const;

    [[nodiscard]] uint64_t getTimestamp(size_t index) const;
    [[nodiscard]] const std::vector<uint64_t>& getTimestamps() const;

    /// @brief Get a vector of all scopes present in the file.
    [[nodiscard]] const std::vector<std::unique_ptr<VCDScope>>& getScopes() const { return scopes_; }

    /// @brief Return a flattened vector of all signals in the file.
    [[nodiscard]] const std::map<std::string, std::unique_ptr<VCDSignal>>& getSignals() const { return signals_; }

    /// @brief Check if hash exists in value map
    [[nodiscard]] bool exists(const std::string& hash) const;

    /// @brief Timescale of the VCD file.
    VCDTimeUnit time_units = VCDTimeUnit::TIME_UNKNOWN;

    /// @brief Multiplier of the VCD file time units.
    uint8_t time_resolution = 0;

    /// @brief Date string of the VCD file
    std::string date;

    /// @brief Version string of the simulator which generated the VCD.
    std::string version;

    /// @brief Current scope nodes of the VCD signals -> To manage parents & children
    VCDScope* current_scope = nullptr;

   private:
    std::map<std::string, std::unique_ptr<VCDSignal>> signals_;
    std::vector<std::unique_ptr<VCDScope>> scopes_;
    std::vector<uint64_t> times_;
};

}  // namespace VCDP_NAMESPACE
