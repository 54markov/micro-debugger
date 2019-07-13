#include <exception/exception.hpp>

Exception::Exception(const std::string& err, const int code)
    : code_(code), err_(err)
{
    ;
}

const char* Exception::what() const noexcept
{
    return err_.c_str();
}

int Exception::code() const noexcept
{
    return code_;
}
