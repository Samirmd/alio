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

#include "client/timer_file_object_decorator.hpp"
#include "xml/xml_node.hpp"

#include <algorithm>
#include <sstream>
#include <iostream>


namespace ALIO
{
FILE *TimerFileObjectDecorator::m_stdout = NULL;

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
};   // TimerFileObjectDecorator

}   // namespace ALIO
