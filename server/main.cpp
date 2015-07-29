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




#include "tools/i_communication.hpp"
#include "tools/message.hpp"
#include "tools/os.hpp"

#include <errno.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
    ALIO::OS::init();

#ifdef USE_MPI
    ICommunication *communication = new MPICommunication(argc, argv);
#endif

    Server *server = new Server(communication);

}   // main

