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

#ifndef HEADER_FILE_OBJECT_INFO_HPP
#define HEADER_FILE_OBJECT_INFO_HPP

#include "client/i_file_object.hpp"

#include <string>
#include <vector>

// Have to use C regex, since support for C++ std::regex does not work
#include <sys/types.h>
#include <regex.h>


/** Stores information for one file name prefix, i.e. which file object
 *  to use.
 */
namespace ALIO
{

class XMLNode;

class FileObjectInfo
{
private:
    typedef enum 
    {
            IO_TYPE_STANDARD = 0x001,     /** Write to disk */
            IO_TYPE_NULL     = 0x002,     /** Write to /dev/null */
            IO_TYPE_REMOTE   = 0x004,     /** Connect to a server. */
            IO_TYPE_DEBUG    = 0x008,     /** Decorator: print debug info.*/
            IO_TYPE_MIRROR   = 0x010,     /** Decorator: mirroring. */
            IO_TYPE_TIMER    = 0x020,     /** Decorator: Collect timing information. */
            IO_TYPE_BUFFERED = 0x040      /** Decorator: use memory as buffer. */
    } IOType; 

    /** The original string for the regex, only to make debugging easier. */
    std::string m_pattern;

    /** If a search or a match should be done. */
    bool m_search;

    /** The regular expression. */
    regex_t  m_regex;

    /** Which IO objects to instantiate. */
    std::vector<IOType> m_io_types;

    /** Stores the original XML node for that particular addon. */
    std::vector<const XMLNode*> m_io_xml_info;

    /** A bit-mask storing all types that are used in the config files. 
     *  It is used to avoid initialising the object for a certain type
     *  more than once. */
    static int m_all_needed_types;

public:
         
                  FileObjectInfo(const XMLNode *node);
                 ~FileObjectInfo();
    static int    init();
    static int    atExit();
    static int    callAllStaticInitFunctions();
    bool          isApplicable(const std::string &filename) const;
    I_FileObject *createFileObject(const std::string &filename) const;
};   // FileObjectInfo

}   // namespace ALIO
#endif
