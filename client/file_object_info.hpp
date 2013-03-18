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
            IO_TYPE_STANDARD,   /** Write to disk */
            IO_TYPE_NULL,       /** Write to /dev/null */
            IO_TYPE_REMOTE,     /** Connect to a server. */
            IO_TYPE_DEBUG,      /** Decorator: print debug info.*/
            IO_TYPE_MIRROR,     /** Decorator: mirroring. */
            IO_TYPE_TIMER       /** Decorator: Collect timing information. */
    } IOType; 

    /** To which pattern this entry applies. */
    std::string m_pattern;

    /** Which IO objects to instantiate. */
    std::vector<IOType> m_io_types;

    /** Stores the original XML node for that particular addon. */
    std::vector<const XMLNode*> m_io_xml_info;


public:
         FileObjectInfo(const XMLNode *node);
    bool isApplicable(const std::string &filename) const;
    I_FileObject *createFileObject(const std::string &filename) const;
};   // FileObjectInfo

}   // namespace ALIO
#endif
