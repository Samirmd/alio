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

#include "../tools/synchronised.hpp"

#include <stdio.h>
#include <string>

class ICommunication;

class Server
{
private:
    FILE *m_file;


    int  m_filedes;

    std::string m_filename;

    /** The interface name to use. */
    std::string m_if_name;

    /** Stores the user's config directory ($HOME/.alio). */
    std::string m_config_dir;

    /** Flag to signal from the receiving thread that a new client 
     *  tries to connect. */
    Synchronised<int> m_new_connection_available;

    /** The object used for all communication. */
    ICommunication *m_communication;

    int handleRequest(char *buffer);

    static void *handleConnectionRequests(void *obj);
    void _handleConnectionRequests();

public:
    Server(const std::string &if_name, ICommunication *communication);
};   // class Server
