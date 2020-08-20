#pragma once

#include <tuple>
#include <string>
#include <vector>
#include <utility>

#include <linux/types.h>


namespace cli {

enum class Commands {
    run,
    quit,
    cont,
    bp,
    dump,
    backtrace,
    variables,
    help,
    unknown
};

typedef std::tuple<Commands, std::intptr_t, std::string, uint32_t> cmd;

class Reader {
    public:
        Reader() = default;
        ~Reader() = default;
        cmd read();
        void help();
};

} // namespace cli
