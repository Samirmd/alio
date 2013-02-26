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
    m_needs_destroy = false;
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
/** Adds an integer value to the message.
 *  \param data The integer value to add.
 */
void Message::addInt(int data)
{
    assert((int)(m_pos + sizeof(int)) <= m_data_size);
    memcpy(m_data+m_pos, &data, sizeof(int)); 
    m_pos+=sizeof(int);
}   // addInt

// ----------------------------------------------------------------------------
/** Extracts an integer value from a message.
 *  \return The extracted integer.
 */
int Message::getInt()
{
    m_pos+=sizeof(int);
    return *(int*)(&m_data[m_pos-sizeof(int)]);
}   // getInt

// ----------------------------------------------------------------------------
/** Adds a short value to the message.
 *  \param data The integer value to add.
 */
void Message::addShort(short data)
{
    assert((int)(m_pos + sizeof(short)) <= m_data_size);
    memcpy(m_data+m_pos, &data, sizeof(short)); 
    m_pos+=sizeof(short);
}   // addShort

// ----------------------------------------------------------------------------
/** Extracts a short value from a message.
 *  \return The short value.
 */
short Message::getShort()
{
    m_pos+=sizeof(short);
    return *(short*)(&m_data[m_pos-sizeof(short)]);
}   // getShort

// ----------------------------------------------------------------------------
/** Adds a floating point value to the message.
 *  \param data Floating point value to add.
 */
void Message::addFloat(const float data)
{
    // The simple approach of using  addInt(*(int*)&data)
    // does not work (at least with optimisation on certain g++ versions,
    // see getFloat for more details)
    int n;
    memcpy(&n, &data, sizeof(float));
    addInt(n);
}   // addFloat
// ----------------------------------------------------------------------------
float Message::getFloat()
{ 
    int i    = getInt();
    float f;
    memcpy(&f, &i, sizeof(int));
    return f;
    // The 'obvious' way:
    // float *f = (float*) &i;
    // return *f;
    // does NOT work, see http://www.velocityreviews.com/forums/showthread.php?t=537336
    // for details
}   // getFloat

// ----------------------------------------------------------------------------
void Message::addString(const std::string &data)
{ 
    int len = data.size()+1;  // copy 0 end byte
    assert((int)(m_pos+len) <=m_data_size);
    memcpy (&(m_data[m_pos]), data.c_str(), len);
    m_pos += len;
}   // addString

// ----------------------------------------------------------------------------
std::string Message::getString()
{
    char *str = (char*) &(m_data[m_pos]);
    int len = strlen(str)+1;
    m_pos += len;
    return std::string(str);
}   // getString

