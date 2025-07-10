#include "VCDParser.hpp"

#include <iostream>
#include <tao/pegtl/contrib/trace.hpp>

#include "VCDActions.hpp"

namespace VCDP_NAMESPACE {

VCDFile* VCDParser::Parse(const std::string& file_path) {
    m_FilePath = file_path;
    m_File = std::make_unique<VCDFile>();

    pegtl::file_input in(file_path);

    switch (m_DebugMode) {
        case DebugMode::Tracer:
            if (!pegtl::standard_trace<lexical::grammar, action>(in, *m_File)) {
                return nullptr;
            }
            break;

        default:
            if (!pegtl::parse<lexical::grammar, action>(in, *m_File)) {
                return nullptr;
            }
            break;
    }

    return m_File.get();
}

void VCDParser::SetDebugMode(DebugMode debug_mode) { m_DebugMode = debug_mode; }

}  // namespace VCDP_NAMESPACE
