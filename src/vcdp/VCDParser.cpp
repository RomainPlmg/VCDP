#include "vcdp/VCDParser.hpp"

#include <tao/pegtl/contrib/trace.hpp>

#include "vcdp/VCDActions.hpp"
#include "vcdp/VCDLexical.hpp"

namespace VCDP_NAMESPACE {

std::unique_ptr<VCDFile> VCDParser::Parse(const std::string& file_path) {
    m_FilePath = file_path;
    m_File = std::make_unique<VCDFile>();
    m_Result.Clear();

    try {
        pegtl::file_input in(file_path);
        if (ActionState state; !pegtl::parse<lexical::full_grammar, action>(in, *m_File, state)) {
            if (!pegtl::parse<lexical::full_grammar, action>(in, *m_File, state)) {
                m_Result.success = false;
                m_Result.errors.emplace_back("Internal parse error...");
            }
        }
    } catch (const pegtl::parse_error& e) {
            m_Result.success = false;
            m_Result.errors.emplace_back("Parse error: " + std::string(e.what()));
        }

        if (!m_Result.success) {
            return nullptr;
        }
        return std::move(m_File);
    }

}  // namespace VCDP_NAMESPACE
