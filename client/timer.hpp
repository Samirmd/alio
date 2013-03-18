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

#ifndef HEADER_TIMER_HPP
#define HEADER_TIMER_HPP

#include <sys/time.h>

class Timer
{
private:
    /** Time the timer was started. */
    double m_start;

    /** Sum of all start/stop intervals. */
    double m_sum;

    double getSeconds()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec + tv.tv_usec*1.0e-6;
    }

public:
    /** Constructor, which initialises this timer to 0. */
    Timer()
    {
        m_sum = 0; m_start = 0; 
    };

    // ------------------------------------------------------------------------
    /** Starts the timer. */
    void start()
    {
        m_start = getSeconds();
    }   // start

    // ------------------------------------------------------------------------
    /** Stops the timer and returns the duration of the current start/stop
     *  interval. This time is also summed. */
    double stop()
    {
        double t = getSeconds()-m_start;
        m_sum += t;
        return t;
    }
    // ------------------------------------------------------------------------
    /** Returns the overall time accumulated in this timer. */
    double getSum() const { return m_sum; }

};   // Timer
#endif
