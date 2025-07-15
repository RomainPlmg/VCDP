#include <argparse/argparse.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "vcdp/VCDP.hpp"

void PrintScope(vcdp::VCDScope* scope, std::vector<bool> last_flags);

int main(int argc, char const* argv[]) {
    // Enable UTF-8 encoding for Windows
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    std::string file_path;
    argparse::ArgumentParser program("vcdp", "0.0.1");
    program.add_argument("vcd_file").help("VCD file to parse").store_into(file_path);
    program.add_argument("--verbose").help("Increase output verbosity").default_value(false).implicit_value(true);
    program.add_argument("-t", "--tree").help("Print the scope & var hierarchy").default_value(false).implicit_value(true);
    program.add_argument("--stats")
        .help("Print stats: number of scopes, variables, changes, duration, etc.")
        .default_value(false)
        .implicit_value(true);

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    vcdp::VCDParser parser;
    auto trace = parser.Parse(file_path);

    if (program["--verbose"] == true) {
        for (const auto& msg : parser.GetResult().errors) {
            std::cout << msg << std::endl;
        }
    }

    if (trace == nullptr) {
        std::cerr << "Parsing fail" << std::endl;
        return 1;
    }

    if (program["--tree"] == true) {
        std::vector<vcdp::VCDScope*> top_scopes;
        for (auto& scope : trace->GetScopes()) {
            // Top scopes
            if (scope->parent == nullptr) top_scopes.push_back(scope.get());
        }

        for (auto& scope : top_scopes) {
            PrintScope(scope, {});
        }
    }

    if (program["--stats"] == true) {
        std::cout << "Scopes: " << trace->GetScopes().size() << std::endl;
        std::cout << "Signals: " << trace->GetSignals().size() << std::endl;
        std::cout << "Timescale: " << trace->time_resolution << vcdp::utils::VCDTimeUnit2String(trace->time_units) << std::endl;
        std::cout << "Date: " << trace->date << std::endl;
        std::cout << "Version: " << trace->version << std::endl;
    }

    return 0;
}

void PrintScope(vcdp::VCDScope* scope, std::vector<bool> last_flags) {
    // Print scope
    for (size_t i = 0; i + 1 < last_flags.size(); ++i) {
        std::cout << (last_flags[i] ? "    " : "│   ");
    }
    if (!last_flags.empty()) {
        std::cout << (last_flags.back() ? "└── " : "├── ");
    }
    std::cout << vcdp::color::MAGENTA << scope->name << vcdp::color::RESET << std::endl;

    // Print signals
    for (size_t i = 0; i < scope->signals.size(); i++) {
        for (size_t j = 0; j < last_flags.size(); ++j) {
            std::cout << (last_flags[j] ? "    " : "│   ");
        }

        bool last_signal = (i == scope->signals.size() - 1) && scope->children.empty();
        std::cout << (last_signal ? "└── " : "├── ");

        auto& signal = scope->signals.at(i);
        std::stringstream bit_index;
        if (signal->lindex > -1)
            bit_index << "[" << signal->lindex << ":" << signal->rindex << "] ";
        else if (signal->rindex > -1)
            bit_index << "[" << signal->rindex << "] ";
        else
            bit_index << " ";

        std::cout << vcdp::color::GREEN << signal->reference << vcdp::color::RESET << " : " << vcdp::color::BLUE
                  << vcdp::utils::VCDVarType2String(signal->type) << " " << vcdp::color::RED << bit_index.str() << vcdp::color::RESET << "("
                  << vcdp::color::YELLOW << "id" << vcdp::color::RESET << ": " << signal->hash << ")" << std::endl;
    }

    // Appel récursif sur les enfants
    for (size_t i = 0; i < scope->children.size(); ++i) {
        bool is_last = (i == scope->children.size() - 1);
        auto new_flags = last_flags;
        new_flags.push_back(is_last);
        PrintScope(scope->children[i], new_flags);
    }
}
