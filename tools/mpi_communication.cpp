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

#include "tools/mpi_communication.hpp"

#ifdef USE_MPI

#include "mpi.h"

MPICommunication::MPICommunication(bool is_server, int argc, char **argv)
                : ICommunication(is_server)
{
    m_port_name      = NULL;
    m_message_length = -1;
    MPI_Init(&argc, &argv);
}   // MPICommunication

// ----------------------------------------------------------------------------
int MPICommunication::init()
{
    return 0;
}   // init

// ----------------------------------------------------------------------------
/** Waits for a client to connect.
 *  \param return 0 If no connection could be established, 1 otherwise.
 */
int MPICommunication::openPort()
{
    m_port_name = new char[MPI_MAX_PORT_NAME+1];
    bzero(m_port_name, MPI_MAX_PORT_NAME);
    MPI_Open_port(MPI_INFO_NULL, m_port_name); 
    printf("Server: open port '%s'.\n", m_port_name);
    return 0;
}   // waitForConnection

// ----------------------------------------------------------------------------
int MPICommunication::waitForConnection()
{
    printf("Server: accepting.\n");
    MPI_Comm_accept(m_port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &m_intercomm);
    printf("Accepted.\n");
    return 0;
}   // waitForConnection

// ----------------------------------------------------------------------------
int MPICommunication::waitForMessage()
{
    printf("probe.\n");fflush(stdout);
    MPI_Probe(0, 1, m_intercomm, &m_status);
    MPI_Get_count(&m_status, MPI_CHAR, &m_message_length);
    return 0;
}   // waitForMessage

// ----------------------------------------------------------------------------
char *MPICommunication::receive()
{
    // If we don't know the size of the message yet, get it using a probe.
    if(m_message_length < 0)
        waitForMessage();

    char *buffer = new char[m_message_length];
    MPI_Recv(buffer, m_message_length, MPI_CHAR, MPI_ANY_SOURCE, 
             MPI_ANY_TAG, m_intercomm, &m_status);
    return buffer;
}   // receive

// ----------------------------------------------------------------------------
int MPICommunication::send(void *buffer, int len, int tag)
{
    printf("MPI Sending tag %d\n", tag);
    if(m_is_server)
        MPI_Send(buffer, len, MPI_CHAR, 0, tag, m_intercomm);
    else
        MPI_Send(buffer, len, MPI_CHAR, 0, tag, m_intercomm);
    return 0;
}   // send

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

#endif // USE_MPI
