#include "client/debug_file_object_decorator.hpp"
#include "client/file_object_info.hpp"
#include "client/mirror.hpp"
#include "client/null_file_object.hpp"
#include "client/remote.hpp"
#include "client/standard_file_object.hpp"
#include "client/timer_file_object_decorator.hpp"

#include "tools/string_utils.hpp"
#include "xml/xml_node.hpp"

#include <assert.h>

namespace AIO
{
FileObjectInfo::FileObjectInfo(const XMLNode *node)
{

    if(!node->get("pattern", &m_pattern))
    {
        printf("No pattern found, aborting.\n");
        exit(-1);
    }
    
    std::string s;
    node->get("master", &s);
    if(s=="standard")
    {
        m_io_types.push_back(IO_TYPE_STANDARD);
    }
    else if(s=="remote")
    {
        m_io_types.push_back(IO_TYPE_REMOTE);
        
    }
    else if(s=="null")
        m_io_types.push_back(IO_TYPE_NULL);
    else
    {
        printf("Invalid master '%s' using standard.\n", s.c_str());
        m_io_types.push_back(IO_TYPE_STANDARD);
    }

    node->get("addons", &s);
    std::vector<std::string> config = StringUtils::split(s, ' ');
    for(unsigned int i=0; i<config.size(); i++)
    {
        if(config[i]=="mirror")
            m_io_types.push_back(IO_TYPE_MIRROR);
        else if(config[i]=="timer")
            m_io_types.push_back(IO_TYPE_TIMER);
        else if(config[i]=="debug")
            m_io_types.push_back(IO_TYPE_DEBUG);
        else
        {
            printf("Invalid config entry '%s' found - aborting.\n",
                   config[i].c_str());
            exit(-1);
        }
    }   // for i < config.size

}   // FileObjectInfo

// ----------------------------------------------------------------------------
bool FileObjectInfo::isApplicable(const std::string &filename) const
{
    return filename.substr(0, m_pattern.size())==m_pattern;
}   // isApplicable
// ----------------------------------------------------------------------------
I_FileObject *FileObjectInfo::createFileObject(const std::string &filename) const
{
    I_FileObject *fo = NULL;
    switch(m_io_types[0])
    {
    case IO_TYPE_STANDARD : fo = new StandardFileObject(); break;
    case IO_TYPE_NULL     : fo = new NullFileObject();     break;
    case IO_TYPE_REMOTE   : Remote::init();
                            fo = new Remote();             break;
    default:
        printf("No final first type found - shouldn't happen.\n");
        exit(-1);
    }

    // Now add all decorators.
    for(unsigned int i=1; i<m_io_types.size(); i++)
    {
        switch(m_io_types[i])
        {
        case IO_TYPE_TIMER  : fo = new TimerFileObjectDecorator(fo); break;
        case IO_TYPE_DEBUG  : fo = new DebugFileObjectDecorator(fo); break;
        case IO_TYPE_MIRROR : fo = new MirrorFileObjectDecorator(fo); break;
        default:
            printf("Incorrect decorator found - ignored.\n");
        }
    }

    fo->setFilename(filename);
    return fo;
}   // createFileObject

}   // namespace AIO
