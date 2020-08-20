#pragma once

#include <memory>
#include <iostream>

#include <exception/exception.hpp>


namespace logger {

constexpr auto microDbg{"[micro-dbg]"};

void input() {
    std::cout << microDbg << " > ";
}

template<typename ... Args>
void printf(const std::string &format, Args ... args) {
    auto size = ::snprintf(nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if (size <= 0) {
        throw Exception("error during formatting");
    }
    std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);
    ::snprintf(buf.get(), size, format.c_str(), args ...);
    std::cout << microDbg << " " << buf.get() << std::endl;
}

void print(const std::string &str) {
    std::cout << microDbg << " " << str << std::endl;
}

template<typename ... Args>
void errorf(const std::string &format, Args ... args) {
    auto size = ::snprintf(nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if (size <= 0) {
        throw Exception("error during formatting");
    }
    std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);
    ::snprintf(buf.get(), size, format.c_str(), args ...);
    std::cerr << microDbg << " " << buf.get() << std::endl;
}

void error(const std::string &str) {
    std::cerr << microDbg << " " << str << std::endl;
}


}; //namespace logger