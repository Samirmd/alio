#include "tools/message.hpp"

#include <string>
#include <math.h>
#include <stdexcept>
#include <assert.h>

/** Creates a message to be sent.
 *  This only initialised the data structures, it does not reserve any memory.
 *  A call to allocate() is therefore necessary.
 *  \param type  The type of the message
 */
Message::Message(MessageType type)
{
    assert(sizeof(int)==4);
    m_type          = type;
    m_pos           = 0;
    m_data_size     = -1;
    m_data          = NULL;
    m_needs_destroy = false;
}   // Message

// ----------------------------------------------------------------------------
/** Handles a received message.
 *  \param buffer The actual data.
 *  \param m      The type of the message. The type is checked via an assert!
 */

Message::Message(char *buffer, int len)
{
    assert(sizeof(int)==4);
    m_data_size = len;
    m_data      = buffer;
    m_type      = (MessageType)m_data[0];
    m_pos       = 1;
    m_needs_destroy = true;
}

// ----------------------------------------------------------------------------
/** Frees the memory allocated for this message. */
Message::~Message()
{
    clear();
}   // ~Message

// ----------------------------------------------------------------------------
/** Frees the memory for a received message.
 *  Calls enet_packet_destroy if necessary (i.e. if the message was received).
 *  The memory for a message created to be sent does not need to be freed, it 
 *  is handled by enet. */
void Message::clear()
{
    if(m_needs_destroy)
        delete m_data;
    m_data = NULL;
    m_needs_destroy = false;
}   // clear

// ----------------------------------------------------------------------------
/** Reserves the memory for a message. 
 *  \param size Number of bytes to reserve.
 */
void Message::allocate(int size)
{
    m_data_size = size+1;
    m_data      = new char[size+1];
    if(!m_data)
    {
        printf("can't allocate '%d' bytes.\n", size+1);
        assert(false);
    }
    m_data[0]   = m_type;
    m_pos       = 1;
}   // allocate

// ----------------------------------------------------------------------------

