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
