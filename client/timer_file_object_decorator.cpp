#include "client/timer_file_object_decorator.hpp"
#include "xml/xml_node.hpp"

#include <algorithm>
#include <sstream>
#include <iostream>


namespace ALIO
{

// ----------------------------------------------------------------------------
TimerFileObjectDecorator::TimerFileObjectDecorator(I_FileObject *parent, 
                                                    const XMLNode *info)
                        : I_FileObjectDecorator(parent, info)
{
    m_timer_data  = NULL;
    m_write_xml   = false;
    m_write_table = true;
    info->get("xml",   &m_write_xml  );
    info->get("table", &m_write_table);
    printf("%d %d\n", m_write_xml, m_write_table);
};   // TimerFileObjectDecorator

}   // namespace ALIO
