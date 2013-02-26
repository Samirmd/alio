#ifndef HEADER_STRING_UTILS_HPP
#define HEADER_STRING_UTILS_HPP

#include <string>
#include <sstream>
#include <vector>

namespace AIO {
namespace StringUtils {

    std::vector<std::string> split(const std::string& s, char c);

    // ------------------------------------------------------------------------
    template<typename TYPE>
    bool parseString(const char* input, TYPE* output)
    {
        std::istringstream conv(input);
        conv >> *output;
        
        // check reading worked correctly and everything was read
        if (conv.fail() || !conv.eof())
        {
            return false;
        }
        return true;
    }
    // ------------------------------------------------------------------------
    template<typename TYPE>
    bool parseString(const std::string& input, TYPE* output)
    {
        return parseString(input.c_str(), output);
    }

    // ------------------------------------------------------------------------
}   // namespace StringUtils
}   // namespace AIO
#endif
