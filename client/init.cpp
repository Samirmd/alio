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

#include "client/config.hpp"
#include "client/standard_file_object.hpp"
#include "client/timer_manager.hpp"
#include "client/debug_file_object_decorator.hpp"
#include "tools/os.hpp"

namespace ALIO
{
    static FILE *stdout_capture;

    double DebugFileObjectDecorator::m_start_time;
/** Constructor of the shared library. It initialises the OS object
 *  which stores pointers to all original file related functions.
 *  Then it initialises the config object, which is responsible
 *  for creating the appropriate FileObject for each file.
 *  Finally it stores the current stdout, since this is
 *  needed in the destructor (and at the time the constructor
 *  is called stdout is already closed and not available anymore).
 */ 
extern "C" void __attribute__((constructor)) my_init(void)
{
    ALIO::DebugFileObjectDecorator::init();
    ALIO::OS::init();
    ALIO::Config::create(/* is_slave*/ true);
    // We need to capture stdout, since the desctructor will be
    // called after stdout is closed, so we could not write
    // anything otherwise
    ALIO::stdout_capture = fopen("/dev/stdout", "w");
}

// ============================================================================
/** The destructor, called when unloading this shared library.
 *  It calls the static output function of the timer to print all
 *  timer output (if there is any).
 */

extern "C" void __attribute__((destructor)) my_exit(void)
{
    ALIO::TimerManager::atExit(ALIO::stdout_capture);
}


}
