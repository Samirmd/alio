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


class ICommunication
{
public:
    /** Dummy constructor. */
    ICommunication() {};

    // ------------------------------------------------------------------------
    /** An init call. 
     *  \return 0 if no error, 1 on error.
     */
    virtual int init() = 0;
    // ------------------------------------------------------------------------
    /** Waits for a client to connect.
     *  \param return 0 If no connection could be established, 1 otherwise.
     */
    virtual int openPort() = 0;
    // ------------------------------------------------------------------------
    /** Returns the port name which is to be used by the clients to connect
     *  to a server. */
    virtual char*getPortName() = 0;
    // ------------------------------------------------------------------------
    /** Waits for a connection.
     *  \return 0 If no error occured, 1 otherwise. */
    virtual int waitForConnection() = 0;
    // ------------------------------------------------------------------------
    /** Waits for a message from a client.
     *  \return 0 If no error occured, 1 otherwise. */
    virtual int waitForMessage() = 0;
    // ------------------------------------------------------------------------
    /** Allocates a buffer to receive an incoming message, and returns a 
     *  pointer to that buffer to the caller. It is the caller's
     *  responsibility to free the buffer when it is not needed anymore. */
    virtual char *receive() = 0;
    // ------------------------------------------------------------------------
    virtual int getMessageLength() = 0;
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
};   // ICommunication
