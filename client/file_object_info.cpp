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

#include "client/file_object_info.hpp"

#include "client/buffered.hpp"
#include "client/debug_file_object_decorator.hpp"
#include "client/mirror.hpp"
#include "client/null_file_object.hpp"
#include "client/remote.hpp"
#include "client/standard_file_object.hpp"
#include "client/timer_file_object_decorator.hpp"

#include "tools/string_utils.hpp"
#include "xml/xml_node.hpp"

#include <assert.h>

namespace ALIO
{

int FileObjectInfo::m_all_needed_types = 0;


/** Static function, called once at startup time. 
 */
int FileObjectInfo::init()
{
    m_all_needed_types = 0;
    return 0;
}   // init

// ----------------------------------------------------------------------------
/** Called when unloading the library. It calls the atExit functions of all
 *  classes that were initialised.
 */
int FileObjectInfo::atExit()
{
    if(m_all_needed_types & IO_TYPE_STANDARD) StandardFileObject       ::atExit();
    if(m_all_needed_types & IO_TYPE_REMOTE  ) Remote                   ::atExit();
    if(m_all_needed_types & IO_TYPE_NULL    ) NullFileObject           ::atExit();
    if(m_all_needed_types & IO_TYPE_MIRROR  ) MirrorFileObjectDecorator::atExit();
    if(m_all_needed_types & IO_TYPE_TIMER   ) TimerFileObjectDecorator ::atExit();
    if(m_all_needed_types & IO_TYPE_DEBUG   ) DebugFileObjectDecorator ::atExit();
    if(m_all_needed_types & IO_TYPE_BUFFERED) BufferedFileObject       ::atExit();
    return 0;
}   // atExit

// ----------------------------------------------------------------------------
/** Calls the static init functions for all file objects and file object
 *  decorators that were requested in the config file.
 */
int FileObjectInfo::callAllStaticInitFunctions()
{
    if(m_all_needed_types & IO_TYPE_STANDARD) StandardFileObject       ::init();
    if(m_all_needed_types & IO_TYPE_REMOTE  ) Remote                   ::init();
    if(m_all_needed_types & IO_TYPE_NULL    ) NullFileObject           ::init();
    if(m_all_needed_types & IO_TYPE_MIRROR  ) MirrorFileObjectDecorator::init();
    if(m_all_needed_types & IO_TYPE_TIMER   ) TimerFileObjectDecorator ::init();
    if(m_all_needed_types & IO_TYPE_DEBUG   ) DebugFileObjectDecorator ::init();
    if(m_all_needed_types & IO_TYPE_BUFFERED) BufferedFileObject       ::init();
    return 0;
}   // callAllStaticInitFunctions

// ----------------------------------------------------------------------------
FileObjectInfo::FileObjectInfo(const XMLNode *node)
{
    if(!node->get("pattern", &m_pattern))
    {
        printf("No pattern found. Use either 'pattern' to define the file pattern.\n");
        exit(-1);
    }
    
    int error = regcomp(&m_regex, m_pattern.c_str(), REG_NOSUB);
    if(error)
    {
        printf("Pattern '%s' is not a valid regular expression: error %d.\n",
               m_pattern.c_str(),error);
        char message[1024];
        regerror(error, &m_regex, message, 1024);
        printf("%s\n", message);
        regfree(&m_regex);
        exit(-1);
    }

    const XMLNode *io = node->getNode("io");
    std::string s;
    io->get("type", &s);

    if(s=="standard")
    {
        m_io_types.push_back(IO_TYPE_STANDARD);
        m_all_needed_types |= IO_TYPE_STANDARD;
    }
    else if(s=="remote")
    {
        m_io_types.push_back(IO_TYPE_REMOTE);
        m_all_needed_types |= IO_TYPE_REMOTE;
    }
    else if(s=="null")
    {
        m_io_types.push_back(IO_TYPE_NULL);
        m_all_needed_types |= IO_TYPE_NULL;
    }
    else if(s=="buffer")
    {
        m_io_types.push_back(IO_TYPE_BUFFERED);
        m_all_needed_types |= IO_TYPE_BUFFERED;
    }
    else
    {
        printf("Invalid io '%s' for pattern '%s'- using standard.\n", 
               s.c_str(), m_pattern.c_str());
        m_io_types.push_back(IO_TYPE_STANDARD);
        m_all_needed_types |= IO_TYPE_STANDARD;
    }

    // Store the XML object so that at instantiation time it is available.
    m_io_xml_info.push_back(io);

    for(unsigned int i=0; i<node->getNumNodes(); i++)
    {
        const XMLNode *addons = node->getNode(i);
        if(!addons)
            continue;   // shouldn't happen - but just in case
        if(addons->getName()=="io")  // already handled above
            continue;
        else if(addons->getName()!="addon")
        {
            printf("Invalid addons node '%s' found in pattern '%s', index %d.\n",
                   addons->getName().c_str(), m_pattern.c_str(), i);
            continue;
        }

        std::string decorator;
        addons->get("type", &decorator);
        if(decorator=="mirror")
        {
            m_io_types.push_back(IO_TYPE_MIRROR);
            m_all_needed_types |= IO_TYPE_MIRROR;
        }
        else if(decorator=="timer")
        {
            m_io_types.push_back(IO_TYPE_TIMER);
            m_all_needed_types |= IO_TYPE_TIMER;
        }
        else if(decorator=="debug")
        {
            m_io_types.push_back(IO_TYPE_DEBUG);
            m_all_needed_types |= IO_TYPE_DEBUG;
        }
        else
        {
            printf("Invalid config entry '%s' found - aborting.\n",
                   decorator.c_str());
            exit(-1);
        }
        m_io_xml_info.push_back(addons);
    }


}   // FileObjectInfo

// ----------------------------------------------------------------------------
/** Destructor, frees memory used by the regex.
 */
FileObjectInfo::~FileObjectInfo()
{
    regfree(&m_regex);
}   // ~FileObjectInfo

// ----------------------------------------------------------------------------
bool FileObjectInfo::isApplicable(const std::string &filename) const
{
    return regexec(&m_regex, filename.c_str(), 0, 0, 0)!=REG_NOMATCH;
}   // isApplicable

// ----------------------------------------------------------------------------
I_FileObject *FileObjectInfo::createFileObject(const std::string &filename) const
{
    std::string s;
    m_io_xml_info[0]->get("type", &s);
    printf("Creating %s %s %s", filename.c_str(), m_io_xml_info[0]->getName().c_str(), s.c_str());
    I_FileObject *fo = NULL;
    switch(m_io_types[0])
    {
    case IO_TYPE_STANDARD : fo = new StandardFileObject(m_io_xml_info[0]); break;
    case IO_TYPE_NULL     : fo = new NullFileObject(m_io_xml_info[0]);     break;
    case IO_TYPE_REMOTE   : fo = new Remote(m_io_xml_info[0]);             break;
    case IO_TYPE_BUFFERED : fo = new BufferedFileObject(m_io_xml_info[0]); break;
    default:
        printf("No final first type found - this shouldn't happen.\n");
        exit(-1);
    }

    // Now add all decorators.
    for(unsigned int i=1; i<m_io_types.size(); i++)
    {
        switch(m_io_types[i])
        {
        case IO_TYPE_TIMER  : 
            fo = new TimerFileObjectDecorator(fo, m_io_xml_info[i]); break;
        case IO_TYPE_DEBUG  : 
            fo = new DebugFileObjectDecorator(fo, m_io_xml_info[i]); break;
        case IO_TYPE_MIRROR : 
            fo = new MirrorFileObjectDecorator(fo, m_io_xml_info[i]); break;
        default:
            printf("Incorrect decorator found - ignored.\n");
        }
    }

    fo->setFilename(filename);
    return fo;
}   // createFileObject

}   // namespace ALIO
