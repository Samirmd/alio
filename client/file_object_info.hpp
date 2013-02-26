#ifndef HEADER_FILE_OBJECT_INFO_HPP
#define HEADER_FILE_OBJECT_INFO_HPP

#include "client/i_file_object.hpp"

#include <string>
#include <vector>


/** Stores information for one file name prefix, i.e. which file object
 *  to use.
 */
namespace AIO
{
class FileObjectInfo
{
private:
    typedef enum 
    {
            IO_TYPE_STANDARD,   /** Write to disk */
            IO_TYPE_NULL,       /** Write to /dev/null */
            IO_TYPE_SERVER,     /** Connect to a server. */
            IO_TYPE_DEBUG,      /** Decorator: print debug info.*/
            IO_TYPE_TIME        /** Decorator: Collect timing information. */
    } IOType; 

    /** To which file prefix this entry applies. */
    std::string m_prefix;

    /** Which IO objects to instantiate. */
    std::vector<IOType> m_io_types;

    std::vector<std::string> splitString(const std::string& s, char c);

public:
         FileObjectInfo(const std::string &config_line);
    bool isApplicable(const std::string &filename) const;
    I_FileObject *createFileObject(const std::string &filename, 
                                   int index) const;
};   // FileObjectInfo

}   // namespace AIO
#endif
