#pragma once

#include <iostream>

#include "Config.hpp"
#include "VCDFile.hpp"

namespace VCDP_NAMESPACE {

struct VCDParseResult {
    bool success = true;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;

    [[nodiscard]] bool HasErrors() const { return !errors.empty(); }
    [[nodiscard]] bool HasWarnings() const { return !warnings.empty(); }

    void PrintErrors(std::ostream& os = std::cerr) const {
        for (const auto& error : errors) {
            os << "Error: " << error << std::endl;
        }
    }

    void PrintWarnings(std::ostream& os = std::cout) const {
        for (const auto& warning : warnings) {
            os << "Warning: " << warning << std::endl;
        }
    }

    void Clear() {
        success = true;
        errors.clear();
        warnings.clear();
    }
};

class VCDParser {
   public:
    std::unique_ptr<VCDFile> Parse(const std::string& file_path);

    [[nodiscard]] const VCDParseResult& GetResult() const { return m_Result; }

   private:
    std::string m_FilePath;
    std::unique_ptr<VCDFile> m_File;
    VCDParseResult m_Result;
};

}  // namespace VCDP_NAMESPACE
