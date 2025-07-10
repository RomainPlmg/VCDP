#pragma once

#include "Config.hpp"
#include "VCDFile.hpp"

namespace VCDP_NAMESPACE {

class VCDParser {
   public:
    enum class DebugMode { None, Tracer };
    VCDFile* Parse(const std::string& file_path);

    void SetDebugMode(DebugMode debug_mode);

   private:
    std::string m_FilePath;
    std::unique_ptr<VCDFile> m_File;
    DebugMode m_DebugMode = DebugMode::None;
};

}  // namespace VCDP_NAMESPACE
