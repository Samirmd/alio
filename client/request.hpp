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

#ifndef HEADER_REQUEST_HPP
#define HEADER_REQUEST_HPP

#include "tools/synchronised.hpp"
#include <pthread.h>

class Request
{
public:
    enum RequestType {RQ_QUIT, RQ_OPEN};
private:
    RequestType m_type;
public:
    Request(RequestType type);
    // ------------------------------------------------------------------------
    RequestType getType() const { return m_type; }
    // ------------------------------------------------------------------------
    virtual void done() {}
};   // class Request

// ============================================================================
class BlockingRequest : public Request
{
private:
    Synchronised<bool>  m_done;
    pthread_cond_t m_signal_done;
public:
    BlockingRequest(RequestType type);
    void wait();
    virtual void done();

};   // class BlockingRequest

#endif
