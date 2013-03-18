//
//    ALIO - ALternative IO library
//    Copyright (C) 2013  Joerg Henrichs
//
//    ALIO is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    ALIO is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with ALIO.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef HEADER_STRING_UTILS_HPP
#define HEADER_STRING_UTILS_HPP

#include <string>
#include <sstream>
#include <vector>

namespace ALIO {
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
}   // namespace ALIO
#endif
