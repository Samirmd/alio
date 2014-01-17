//
//    ALIO - ALternative IO library
//    Copyright (C) 2014  Joerg Henrichs
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

#include "client/request.hpp"

#include <stdio.h>

Request::Request(RequestType type)
{
    m_type = type;
}   // Request::Request

// ========================================================================
BlockingRequest::BlockingRequest(RequestType type)
               : Request(type)
{
    pthread_cond_init (&m_signal_done,  NULL);
    m_done.setAtomic(false);
};   // BlockingRequest

// ----------------------------------------------------------------------------
void BlockingRequest::wait()
{
    printf("Request %d wait called.\n", getType());
    m_done.lock();
    while(!m_done.getData())
    {
        printf("Request %d waiting.\n", getType());
        pthread_cond_wait(&m_signal_done, m_done.getMutex());
    }
    m_done.unlock();
    printf("Request %d wait exiting.\n", getType());
}   // wait

// ----------------------------------------------------------------------------
void BlockingRequest::done()
{
    printf("Request %d done called.\n", getType());
    m_done.setAtomic(true);
    pthread_cond_signal(&m_signal_done);
    printf("Request %d done exiting.\n", getType());
}   // done
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------


