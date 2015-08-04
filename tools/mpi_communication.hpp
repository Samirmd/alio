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


#ifndef HEADER_MPI_COMMUNICATION
#define HEADER_MPI_COMMUNICATION

#ifdef USE_MPI


#include "tools/i_communication.hpp"

#include "mpi.h"

class MPICommunication : public ICommunication
{
private:
    /** The port name used when opening a port. */
    char *m_port_name;

    /** Communicator for the server. */
    MPI_Comm m_intercomm;

    /** Status of last message received. */
    MPI_Status m_status;

    /** Length of incoming message. */
    int m_message_length;
public:
                  MPICommunication(bool is_server, int argc=0,
                                   char **argv=NULL);
    virtual int   init();
    virtual int   openPort();
    virtual int   waitForConnection();
    virtual int   waitForMessage();
    virtual char *receive();
    virtual int   send(void *buffer, int len, int tag);
    // ------------------------------------------------------------------------
    /** Returns the length of an incoming message. */
    virtual int   getMessageLength()
    {
        return m_message_length;
    }
    // ------------------------------------------------------------------------
    /** Allocates a buffer to receive an incoming message, and returns a 
     *  pointer to that buffer to the caller. It is the caller's
     *  responsibility to free the buffer when it is not needed anymore. */

    virtual char *getPortName()
    {
        return m_port_name;
    }
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------

};   // MPICommunication

#endif   // USE_MPI

#endif // HEADER_MPI_COMMUNICATION
