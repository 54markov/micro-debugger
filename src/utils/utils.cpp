#include <sstream>

#include <utils/utils.hpp>


std::vector<std::string> utils::split(const std::string &s, const char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

bool utils::isSuffix(const std::string &s, const std::string &of) {
    if (s.size() == 0 || of.size() == 0) {
        return false;
    }

    if (s.size() > of.size()) {
        return false;
    }

    const auto diff = of.size() - s.size();
    return std::equal(s.begin(), s.end(), of.begin() + diff);
}
