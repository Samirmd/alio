#include "client/debug_file_object_decorator.hpp"
#include "client/file_object_info.hpp"
#include "client/null_file_object.hpp"
#include "client/standard_file_object.hpp"

#include <assert.h>

namespace AIO
{
FileObjectInfo::FileObjectInfo(const std::string &config_line)
{
    std::vector<std::string> entries = splitString(config_line, '|');
    if(entries.size()!=2)
    {
        printf("Invalid config line:\n%s\nAborting.\n",
               config_line.c_str());
        exit(-1);
    }
    m_prefix = entries[0];

    std::vector<std::string> config = splitString(entries[1], ' ');
    for(unsigned int i=0; i<config.size(); i++)
    {
        if(config[i]=="standard")
            m_io_types.push_back(IO_TYPE_STANDARD);
        else if(config[i]=="server")
            m_io_types.push_back(IO_TYPE_SERVER);
        else if(config[i]=="null")
            m_io_types.push_back(IO_TYPE_NULL);
        else if(config[i]=="time")
            m_io_types.push_back(IO_TYPE_TIME);
        else if(config[i]=="debug")
            m_io_types.push_back(IO_TYPE_DEBUG);
        else
        {
            printf("Invalid config entry '%s' found - aborting.\n",
                   config[i].c_str());
            exit(-1);
        }
    }   // for i < config.size

    if(m_io_types.size()==0)
        printf("Warning: no information for pattern '%s' given - will use standard IO.\n",
                m_prefix.c_str());
    else
    {
        if( m_io_types[0]!=IO_TYPE_STANDARD &&
            m_io_types[0]!=IO_TYPE_SERVER   &&
            m_io_types[0]!=IO_TYPE_NULL       )
        {
            fprintf(stderr, "Incorrect first IO type: '%s' - aborting.\n",
                    config[0].c_str());
            exit(-1);
        }
    }
}   // FileObjectInfo

// ----------------------------------------------------------------------------
/** Splits a string into substrings separated by a certain character, and
*  returns a std::vector of all those substring. E.g.:
*  split("a b=c d=e",' ')  --> ["a", "b=c", "d=e"]
*  \param s The string to split.
*  \param c The character  by which the string is split.
*/
std::vector<std::string> FileObjectInfo::splitString(const std::string& s, char c)
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

// ----------------------------------------------------------------------------
bool FileObjectInfo::isApplicable(const std::string &filename) const
{
    return filename.substr(0, m_prefix.size())==m_prefix;
}   // isApplicable
// ----------------------------------------------------------------------------
I_FileObject *FileObjectInfo::createFileObject(const std::string &filename, 
                                               int index) const
{

    I_FileObject *fo = NULL;
    switch(m_io_types[0])
    {
    case IO_TYPE_STANDARD : fo = new StandardFileObject(); break;
    case IO_TYPE_NULL     : fo = new NullFileObject();     break;
    case IO_TYPE_SERVER   : fo = new StandardFileObject(); break;
    default:
        printf("No final first type found - shouldn't happen.\n");
        exit(-1);
    }

    // Now add all decorators.
    for(unsigned int i=1; i<m_io_types.size(); i++)
    {
        switch(m_io_types[i])
        {
        case IO_TYPE_TIME  : fo = new StandardFileObject(); break;
        case IO_TYPE_DEBUG : fo = new DebugFileObjectDecorator(fo); break;
        default:
            printf("Incorrect decorator found - ignored.\n");
        }
    }

    fo->setData(filename, index);
    return fo;
}   // create

}   // namespace AIO