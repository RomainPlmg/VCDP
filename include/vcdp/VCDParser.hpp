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
    void parseHeader(std::ifstream& stream, VCDFile* file, const std::string& file_path);
    void parseValueChange(std::ifstream& stream, VCDFile* file, const std::string& file_path);
    void parse(const std::string& file_path, VCDFile* file);

    [[nodiscard]] const VCDParseResult& GetResult() const { return result_; }

   private:
    VCDParseResult result_;
    VCDFile* file_ = nullptr;

    static size_t findNextLine(const std::string& chunk, size_t start);
    void parseValueChangeLine(const std::string& line) const;
};

}  // namespace VCDP_NAMESPACE
