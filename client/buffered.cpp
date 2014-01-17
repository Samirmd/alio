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

#include "buffered.hpp"

#include "client/request.hpp"

#include <pthread.h>

namespace ALIO
{

    pthread_cond_t BufferedFileObject::m_request_signal;
    Synchronised< std::vector<Request*> > BufferedFileObject::m_request_queue;    
    int BufferedFileObject::init()
    {

        pthread_cond_init(&m_request_signal, NULL);


        pthread_attr_t  attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
         // Should be the default, but just in case:
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        pthread_t *thread_id = new pthread_t();
        int error = pthread_create(thread_id, &attr,
                                   &BufferedFileObject::serverMainLoop, NULL);

        return 0;
    }   // init

    // ------------------------------------------------------------------------
    /** No static atExit function needed. */
    int BufferedFileObject::atExit()
    {
        BlockingRequest *quit = new BlockingRequest(Request::RQ_QUIT);
        addRequest(quit);
        quit->wait();
        pthread_cond_destroy(&m_request_signal);
        return 0; 
    }
    // ------------------------------------------------------------------------
    BufferedFileObject::BufferedFileObject(I_FileObject *fo, 
                                           const XMLNode *info)
                      : BaseFileObject(info)
    {
        m_file     = NULL; 
    };   // BufferedFileObject
    // ------------------------------------------------------------------------
    BufferedFileObject::~BufferedFileObject()
    {
    }   // ~BufferedFileObject
    // ------------------------------------------------------------------------
    void BufferedFileObject::addRequest(Request *request)
    {
        m_request_queue.lock();
        m_request_queue.getData().push_back(request);
        pthread_cond_signal(&m_request_signal);
        m_request_queue.unlock();
    }   // addRequest

    // ========================================================================
    /** This is a separate server thread, that will try to write back stored
     *  pages. It will use alio itself for all IO, so it can make use of all
     *  those features.
     */
    void *BufferedFileObject::serverMainLoop(void *obj)
    {
        printf("Thread started - hoooray\n\n");
        
        m_request_queue.lock();
        while(1)
        {
            bool empty = m_request_queue.getData().empty();
            // Wait in cond_wait for a request to arrive. The 'while' is necessary
            // since "spurious wakeups from the pthread_cond_wait ... may occur"
            // (pthread_cond_wait man page)!
            while(empty)
            {
                pthread_cond_wait(&m_request_signal, m_request_queue.getMutex());
                empty = m_request_queue.getData().empty();
            }
            std::vector<Request*>::iterator i = m_request_queue.getData().begin();
            Request *request = *i;
            m_request_queue.getData().erase(i);
            m_request_queue.unlock();
            printf("Received request %d\n", request->getType());
            if(request->getType()==Request::RQ_QUIT) 
            {
                request->done();
                break;
            }
        }   // while !m_abort
        printf("Thread now exiting.\n");

        return NULL;
    }   // server

};   // namespace ALIO
