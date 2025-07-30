#include <argparse/argparse.hpp>
#include <atomic>
#include <csignal>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "vcdp/VCDP.hpp"

constexpr const char* SECTION_SEPARATOR = "\n\n";

void PrintScope(const vcdp::VCDScope* scope, std::vector<bool> last_flags);
void PrintSectionBanner(const std::string& title);

int main(const int argc, char const* argv[]) {
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
    program.add_argument("--symbol").help("Signal name to observe").nargs(1);

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << vcdp::color::RED << err.what() << vcdp::color::RESET << std::endl;
        std::cerr << vcdp::color::RED << program << vcdp::color::RESET << std::endl;
        return 1;
    }

    vcdp::VCDParser parser;
    vcdp::VCDFile trace;

    parser.parse(file_path, &trace);

    if (program["--verbose"] == true) {
        for (const auto& msg : parser.GetResult().errors) {
            std::cerr << vcdp::color::RED << msg << vcdp::color::RESET << std::endl;
        }
    }

    if (program.is_used("--tree")) {
        PrintSectionBanner("VCD SST");

        std::vector<vcdp::VCDScope*> top_scopes;
        for (auto& scope : trace.getScopes()) {
            // Top scopes
            if (scope->parent == nullptr) top_scopes.push_back(scope.get());
        }

        for (const auto& scope : top_scopes) {
            PrintScope(scope, {});
        }

        std::cout << SECTION_SEPARATOR;
    }

    if (program.is_used("--stats")) {
        PrintSectionBanner("VCD Stats");

        if (program["--verbose"] == true) {
            for (const auto& msg : parser.GetResult().errors) {
                std::cerr << vcdp::color::RED << msg << vcdp::color::RESET << std::endl;
            }
        }

        std::cout << "Scopes: " << trace.getScopes().size() << std::endl;
        std::cout << "Signals: " << trace.getSignals().size() << std::endl;
        std::cout << "Timescale: " << trace.time_resolution << vcdp::utils::vcdTimeUnit2String(trace.time_units) << std::endl;
        std::cout << "Date: " << trace.date << std::endl;
        std::cout << "Version: " << trace.version << std::endl;
        std::cout << SECTION_SEPARATOR;
    }

    if (program.is_used("--symbol")) {
        const auto symbol = program.get<std::string>("--symbol");

        for (const auto& timestamp : trace.getTimestamps()) {
            std::cout << timestamp << std::endl;
        }
    }

    std::cout << "\nPress any key to exit...";
    std::cin.get();

    return 0;
}

void PrintScope(const vcdp::VCDScope* scope, std::vector<bool> last_flags) {
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
        for (auto&& last_flag : last_flags) {
            std::cout << (last_flag ? "    " : "│   ");
        }
        const bool last_signal = (i == scope->signals.size() - 1) && scope->children.empty();
        std::cout << (last_signal ? "└── " : "├── ");
        auto& signal = scope->signals.at(i);
        std::stringstream bit_index;
        if (signal->lindex > -1)
            bit_index << "[" << signal->lindex << ":" << signal->rindex << "] ";
        else if (signal->rindex > -1)
            bit_index << "[" << signal->rindex << "] ";
        else
            bit_index << "";

        std::cout << vcdp::color::GREEN << signal->reference << vcdp::color::RESET << " : " << vcdp::color::BLUE
                  << vcdp::utils::vcdVarType2String(signal->type) << " " << vcdp::color::RED << bit_index.str() << vcdp::color::RESET << "("
                  << vcdp::color::YELLOW << "id" << vcdp::color::RESET << ": " << signal->hash << ")" << std::endl;
    }

    // Recursive for children
    for (size_t i = 0; i < scope->children.size(); ++i) {
        const bool is_last = (i == scope->children.size() - 1);
        auto new_flags = last_flags;
        new_flags.push_back(is_last);
        PrintScope(scope->children[i], new_flags);
    }
}

void PrintSectionBanner(const std::string& title) {
    std::cout << "==========================================\n";
    std::cout << "[ " << title << " ]\n";
    std::cout << "------------------------------------------\n";
}
