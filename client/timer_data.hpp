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

#ifndef HEADER_TIMER_DATA_HPP
#define HEADER_TIMER_DATA_HPP

#include "client/i_file_object_decorator.hpp"
#include "client/timer.hpp"

#include <errno.h>
#include <math.h>

#include <stdint.h>
#include <string>
#include <vector>

namespace ALIO
{

/** That's the class that's actually be used as timer for each 
 *  file object. */
class TimerData
{
public:
private:
    /** The filename. Note that this is usually */
    std::string   m_name;
    
    /** The actual timer. */
    std::vector<Timer> m_timers;
    
    /** How often a function was called. */
    std::vector<unsigned long> m_count;
    
    /** E.g. number of bytes read or written. */
    std::vector<unsigned long> m_amount;

    /** Minimum amount. */
    std::vector<unsigned long> m_min_amount;

    /** Maximum amount. */
    std::vector<unsigned long> m_max_amount;

    /** True if data should be written in xml. */
    bool m_do_xml;

    /** True if data should be written as an ASCII table. */
    bool m_do_table;

public:
    TimerData(unsigned int count, const std::string &name, 
              bool write_xml, bool write_table)
    {
        m_timers.resize(count);
        m_count.resize(count);
        m_amount.resize(count);
        m_min_amount.resize(count);
        m_max_amount.resize(count);

        m_name     = name;
        m_do_xml   = write_xml;
        m_do_table = write_table;
        for(unsigned int i=0; i<count; i++)
        {
            m_count[i]  = 0;
            m_amount[i] = 0;
            m_min_amount[i] = UINT_LEAST32_MAX;
            m_max_amount[i] = 0;
        }
    }
    // --------------------------------------------------------------------
    void setName(const std::string &name) 
    { 
        m_name = name;
    }
    // --------------------------------------------------------------------
    const std::string& getName() const { return m_name; }
    // --------------------------------------------------------------------
    void start(int n) { m_count[n]++; m_timers[n].start(); }
    // --------------------------------------------------------------------
    void start(int n, unsigned long amount) 
    { 
        m_amount[n] += amount;
        m_count[n]++; 
        m_timers[n].start(); 
    }   // start
    // --------------------------------------------------------------------
    double stop(int n) { return m_timers[n].stop(); }
    // --------------------------------------------------------------------
    double stop(int n, unsigned long amount) 
    { 
        m_amount[n] += amount;
        if(amount < m_min_amount[n])
            m_min_amount[n] = amount;
        if(amount > m_max_amount[n])
            m_max_amount[n] = amount;

        return m_timers[n].stop(); 
    }
    // --------------------------------------------------------------------
    unsigned long getCount(int n) const { return m_count[n]; }
    // --------------------------------------------------------------------
    unsigned long getAmount(int n) const { return m_amount[n]; }
    // --------------------------------------------------------------------
    unsigned long getMinAmount(int n) const { return m_min_amount[n]; }
    // --------------------------------------------------------------------
    unsigned long getMaxAmount(int n) const { return m_max_amount[n]; }
    // --------------------------------------------------------------------
    double getTime(int n) const { return m_timers[n].getSum(); }
    // --------------------------------------------------------------------
    bool writeXML() const { return m_do_xml; }
    // --------------------------------------------------------------------
    bool writeTable() const { return m_do_table; }
};

}   // namespace ALIO
#endif
