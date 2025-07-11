#include "vcdp/VCDParser.hpp"

#include <iostream>
#include <tao/pegtl/contrib/trace.hpp>

#include "vcdp/VCDActions.hpp"
#include "vcdp/VCDLexical.hpp"
#include "vcdp/VCDValidationError.hpp"

namespace VCDP_NAMESPACE {

VCDFile* VCDParser::Parse(const std::string& file_path) {
    m_FilePath = file_path;
    m_File = std::make_unique<VCDFile>();

    m_Result.Clear();

    pegtl::file_input in(file_path);

    try {
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
    } catch (const VCDValidationError& e) {
        m_Result.success = false;
        m_Result.errors.emplace_back(e.what());
        return nullptr;
    } catch (const tao::pegtl::parse_error& e) {
        m_Result.success = false;
        m_Result.errors.push_back("Parse error: " + std::string(e.what()));
        return nullptr;
    }

    return m_File.get();
}

void VCDParser::SetDebugMode(DebugMode debug_mode) { m_DebugMode = debug_mode; }

}  // namespace VCDP_NAMESPACE
