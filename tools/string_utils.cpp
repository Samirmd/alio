#include "string_utils.hpp"

#include <assert.h>

namespace AIO {
namespace StringUtils {

// ----------------------------------------------------------------------------
/** Splits a string into substrings separated by a certain character, and
*  returns a std::vector of all those substring. E.g.:
*  split("a b=c d=e",' ')  --> ["a", "b=c", "d=e"]
*  \param s The string to split.
*  \param c The character  by which the string is split.
*/
std::vector<std::string> split(const std::string& s, char c)
{
    std::vector<std::string> result;

    try
    {
        std::string::size_type start=0;
        while(start!=std::string::npos && start<s.size())
        {
            std::string::size_type i=s.find(c, start);
            if (i!=std::string::npos)
            {
                result.push_back(std::string(s,start, i-start));
                start=i+1;
            }
            else   // end of string reached
            {
                result.push_back(std::string(s,start));
                start = i;
            }
        }   // while start != npos
        return result;
    }
    catch (std::exception& e)
    {
        fprintf(stderr,
            "Fatal error in split(std::string) : %s @ line %i : %s\n",
            __FILE__, __LINE__, e.what());
        printf("Splitting %s\n", s.c_str());

        for (int n=0; n<(int)result.size(); n++)
        {
            printf("Split : %s\n", result[n].c_str());
        }

        assert(false); // in debug mode, trigger debugger
        exit(1);
    }
}   // split

}   // namspace StringUtils
}   // namespace AIO
