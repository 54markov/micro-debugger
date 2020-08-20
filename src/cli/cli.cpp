#include <iostream>
#include <charconv>

#include <cli/cli.hpp>
#include <utils/utils.hpp>
#include <logger/logger.hpp>


cli::cmd cli::Reader::read() {
    cli::Commands cmd{cli::Commands::unknown};
    std::intptr_t addr{0};
    std::string source{""};
    uint32_t line{0};

    std::string input;

    logger::input();
    std::getline(std::cin, input);

    if (input.compare("help") == 0) {
        cmd = cli::Commands::help;
        help();
    } else if (input.compare("run") == 0) {
        cmd = cli::Commands::run;
    } else if (input.compare("quit") == 0) {
        cmd = cli::Commands::quit;
    } else if (input.compare("cont") == 0) {
        cmd = cli::Commands::cont;
    } else if (input.compare("dump") == 0) {
        cmd = cli::Commands::dump;
     } else if (input.compare("backtrace") == 0) {
        cmd = cli::Commands::backtrace;
    } else if (input.compare("variables") == 0) {
        cmd = cli::Commands::variables;
    } else if (auto found = input.find("bp "); found != std::string::npos) {
        auto v = utils::split(input, ' ');
        if (v.size() == 2) {
            if (auto found = v.at(1).find("0x"); found != std::string::npos) {
                addr = std::stol(v.at(1), 0, 16);
                cmd = cli::Commands::bp;
            } else if (auto found = v.at(1).find(":"); found != std::string::npos) {
                auto str = utils::split(v.at(1), ':');
                source = str.at(0);
                std::from_chars(str.at(1).data(), str.at(1).data() + str.at(1).size(), line);
                cmd = cli::Commands::bp;
            }
        }
    }
    return std::make_tuple(cmd, addr, source, line);
}

void cli::Reader::help() {
    logger::print("\nrun       - start debug\n"
    "quit      - stop debug\n"
    "cont      - continue debug\n"
    "bp        - step breakpoint\n"
    "            bp <0xHEXADDR>\n"
    "            bp <PATH_TO:LINE>\n"
    "dump      - dump registers\n"
    "backtrace - returns a backtrace for the calling program\n"
    "variables - prints variables value for the current function by the breakpoint");
}
