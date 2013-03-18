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

#ifndef HEADER_TIMER_MANAGER_HPP
#define HEADER_TIMER_MANAGER_HPP

#include "client/timer_data.hpp"

#include <vector>

namespace ALIO
{

/** That's the class that's actually be used as timer for each 
 *  file object. */
class TimerManager
{
    typedef std::vector<ALIO::TimerData*> AllTimerDataType;

    /** This static object must be a pointer to the class, not a class. If it
     *  would be the class, the destructor would be called before the destructor
     *  of the shared library, meaning we can't print the stats anymore.
     */
    static AllTimerDataType *m_all_timer_data;

    static void writeAsciiTable(FILE *out);
    static void writeXML(FILE *out);

public:

    static void atExit(FILE *out);

    static TimerData *getTimer(unsigned int count, 
                               const std::string &filename,
                               bool write_xml, bool write_table);


};   // class Timermanager

}   // namespace ALIO
#endif
