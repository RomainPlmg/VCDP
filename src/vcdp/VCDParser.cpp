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

    try {
        pegtl::file_input in(file_path);
        if (!pegtl::parse<lexical::full_grammar, action>(in, *m_File)) {
            m_Result.success = false;
            m_Result.errors.push_back("Internal parse error...");
        }

    } catch (const VCDValidationError& e) {
        m_Result.success = false;
        m_Result.errors.push_back("Parse error: " + std::string(e.what()));
    } catch (const tao::pegtl::parse_error& e) {
        m_Result.success = false;
        m_Result.errors.push_back("Parse error: " + std::string(e.what()));
    }

    if (!m_Result.success) {
        return nullptr;
    }
    return m_File.get();
}

}  // namespace VCDP_NAMESPACE
