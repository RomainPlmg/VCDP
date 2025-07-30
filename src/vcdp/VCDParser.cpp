#include "vcdp/VCDParser.hpp"

#include <array>
#include <fstream>
#include <tao/pegtl/contrib/trace.hpp>

#include "vcdp/VCDActions.hpp"
#include "vcdp/VCDLexical.hpp"

namespace VCDP_NAMESPACE {

void VCDParser::parseHeader(std::ifstream& stream, VCDFile* file, const std::string& file_path) {
    file_ = file;
    result_.Clear();

    // Read until $enddefinitions
    std::string header;
    std::string line;
    bool found_enddefinitions = false;
    bool found_end = false;

    while (std::getline(stream, line)) {
        header += line + '\n';

        // Find $enddefinitions
        if (!found_enddefinitions) {
            if (line.find("$enddefinitions") != std::string::npos) {
                found_enddefinitions = true;
            }
        }

        // Find end following $enddefinitions
        if (found_enddefinitions && line.find("$end") != std::string::npos) {
            found_end = true;
            break;
        }
    }

    if (!found_end) {
        result_.errors.emplace_back("Parse error: Missing $end after $enddefinitions");
        return;
    }

    try {
        pegtl::memory_input<> in(header, file_path);
        if (ActionState state; !pegtl::parse<lexical::declaration_section, action>(in, *file_, state)) {
            result_.success = false;
            result_.errors.emplace_back("Internal parse error...");
        }
    } catch (const pegtl::parse_error& e) {
        result_.success = false;
        result_.errors.emplace_back("Parse error: " + std::string(e.what()));
    }

    file_ = nullptr;
}

void VCDParser::parseValueChange(std::ifstream& stream, VCDFile* file, const std::string& file_path) {
    file_ = file;
    result_.Clear();
    constexpr size_t BUFFER_SIZE = 64 * 1024;  // 64 Ko chunks
    std::array<char, BUFFER_SIZE> buffer{};
    std::string leftover;  // For cutted lines, caused by chunking

    while (stream.read(buffer.data(), BUFFER_SIZE) || stream.gcount() > 0) {
        const size_t byte_read = stream.gcount();

        // Combine leftover + the new chunk
        std::string chunk = leftover + std::string(buffer.data(), byte_read);
        leftover.clear();

        // Parse line by line in the chunk
        size_t pos = 0;
        size_t line_start = 0;

        while ((pos = findNextLine(chunk, line_start)) != std::string::npos) {
            std::string line = chunk.substr(line_start, pos - line_start);

            // Parse the line
            parseValueChangeLine(line);

            line_start = pos + 1;
        }

        // Keep the incomplete line for the next chunk
        if (line_start < chunk.size()) {
            leftover = chunk.substr(line_start);
        }
    }

    // Parse the last line of the file (no '\n')
    if (!leftover.empty()) {
        // Parse the line
        parseValueChangeLine(leftover);
    }

    file_ = nullptr;
}

void VCDParser::parse(const std::string& file_path, VCDFile* file) {
    result_.Clear();

    std::ifstream stream(file_path, std::ios::binary);

    parseHeader(stream, file, file_path);
    parseValueChange(stream, file, file_path);
}

size_t VCDParser::findNextLine(const std::string& chunk, size_t start) {
    const size_t lf_pos = chunk.find('\n', start);
    const size_t cr_pos = chunk.find('\r', start);
    if (lf_pos == std::string::npos && cr_pos == std::string::npos) {
        return std::string::npos;
    }

    if (lf_pos == std::string::npos) return cr_pos;
    if (cr_pos == std::string::npos) return lf_pos;

    return std::min(lf_pos, cr_pos);
}

void VCDParser::parseValueChangeLine(const std::string& line) const {
    if (line.empty() || line[0] == '$') return;  // Skip comments/dump commands

    // Timestamp
    if (line[0] == '#') {
        const uint64_t current_time = std::stoull(line.substr(1));
        file_->addTimestamp(current_time);
    }
}

}  // namespace VCDP_NAMESPACE
