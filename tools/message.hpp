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

#ifndef HEADER_MESSAGE_HPP
#define HEADER_MESSAGE_HPP

#include <cstring>
#include <string>
#include <assert.h>
#include <sys/types.h>

#include <stdio.h>

using std::memcpy;

class Message
{ 
public:
    typedef enum
    {
        MSG_FOPEN,      MSG_FOPEN64,   MSG_FSEEK,
        MSG_FSEEKO,     MSG_FSEEKO64,  MSG_FTELL,
        MSG_FTELLO,     MSG_FTELLO64,  MSG_FERROR,
        MSG_FWRITE,     MSG_FREAD,     MSG_FEOF,
        MSG_FGETS,      MSG_FCLOSE,    MSG_OPEN,
        MSG_OPEN64,     MSG___XSTAT,   MSG___FXSTAT,
        MSG___FXSTAT64, MSG___LXSTAT,  MSG_LSEEK,
        MSG_LSEEK64,    MSG_WRITE,     MSG_READ,
        MSG_CLOSE,      MSG_RENAME,  
        MSG_QUIT,
        MSG_FSEEK_ANSWER, MSG_FREAD_ANSWER
    } MessageType;

private:
    /** The message type. */
    MessageType   m_type;

    /** The actual content of the messsage. */
    char         *m_data;

    /** Size of the data. */
    int           m_data_size;
public:
    /** Simple stack counter for constructing packet data. */
    size_t        m_pos;

    /** Local index of the file object that is responsible for this 
     *  message. */
    int           m_index;
protected:
    // only received messages need to be destroyed
    bool          m_needs_destroy;

public:

    /** Template add function, that adds one element of the given type 
     *  to the message. The parameter n is in case of char * etc, where
     *  an additional call to strlen would be necessary. So by adding the
     *  size as parameter, at least one call is removed.
     */
    template <typename TYPE>
    void add(const TYPE &data)
    {
        assert(m_pos + sizeof(TYPE) <= m_data_size);
        memcpy(m_data+m_pos, &data, sizeof(TYPE));
        m_pos += sizeof(TYPE);
    };  // add(TYPE, n)

    // ------------------------------------------------------------------------
    /** Specialisation for null-terminated character arrays. */
    void add(const char *c, size_t n) 
    {
        assert((int)(m_pos+n)<=m_data_size);
        memcpy(m_data+m_pos, c, n);
        m_pos+=n;
    }   // add(const char*, n)

    // ------------------------------------------------------------------------
    /** Specialisation for arbitrary binary data of a given length. */
    void add(const void *p, size_t n)
    {
        assert(m_pos+n<=m_data_size);
        memcpy(m_data+m_pos, p, n);
        m_pos+=n;
    }   // add(void*, n)

    // ------------------------------------------------------------------------
    /** Specialisation for std::strings. */
    void add(const std::string &data)
    { 
        size_t n = data.size()+1;   // 1 byte for end NULL.
        assert(m_pos+n<=m_data_size);
        memcpy (m_data+m_pos, data.c_str(), n);
        m_pos += n;
    }   // add(std::string)

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    /** Template class to extract an arbitrary data type from a message. */
    template <class TYPE>
    void get(TYPE *data)
    {
        memcpy((void*)data, m_data+m_pos, sizeof(TYPE));
        m_pos += sizeof(TYPE);
    }
    // ------------------------------------------------------------------------
    /** Specialisation for std::strings. */
    void get(std::string *name)
    {
        char *str = m_data + m_pos;
        int len   = strlen(str)+1;
        *name = m_data+m_pos;
        m_pos += len;
    }
    // ------------------------------------------------------------------------
    /** Just returns a char pointer to the current position in the buffer. */
    char* get()
    {
        return m_data+m_pos;
    }
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    template <class TYPE>
    size_t getSize(const TYPE &data)
    {
        return sizeof(data);
    }
    // ------------------------------------------------------------------------
    size_t getSize(const std::string &data)
    {
        // add 1 since it is stored as a zero terminated character array
        return data.size()+1;
    }
    // ------------------------------------------------------------------------
    size_t getSize(const char* data)
    {
        return strlen(data)+1;  // 0 byte at end.
    }
    // ------------------------------------------------------------------------

public:
                 Message(MessageType m, int index);
                 Message(MessageType m, const void *buffer, int len);
                ~Message();
    void         clear();
    void         allocate(size_t size);
    // ------------------------------------------------------------------------
    /** Returns the message type. */
    MessageType  getType() const   { return m_type; }
    // ------------------------------------------------------------------------
    /** Returns the data size - excluding the 1 byte type. */
    int          getLen() const { return m_data_size; }
    // ------------------------------------------------------------------------
    /** Returns a pointer to the actual data - excluding the 1 byte type. */
    char*        getData() {return m_data; }
    // ------------------------------------------------------------------------
    /** Returns the local index of the file object responsible for this
     *  message. */
    int getIndex() const 
    {
        return m_index;
    }   // getIndex

};   // Message

// ============================================================================
/** Convenient class that takes 0 arguments (i.e. only a type, no additional
 *  information.
 */
template <Message::MessageType MT>
class Message0 : public Message
{
public:
    Message0(int index, bool dont_allocate=false) : Message(MT, index)
    {
        if(!dont_allocate)
            allocate(0);
    }   // class Message0
    // ------------------------------------------------------------------------
    Message0(const void *buffer, int n) : Message(MT, buffer, n)
    {
        assert(getType()==MT);
    }
};   // class Message0

// ============================================================================
/** Convenient template class that takes 1 argument. Using a template has
 *  the advantage that type checking can be done at compile time.
 */
template <Message::MessageType MT, typename T1>
class Message1 : public Message
{

public:
    Message1(int index, const T1 &t1) : Message(MT, index)
    {
        size_t n1 = getSize(t1);
        allocate(n1);
        add(t1);
    }   // Message1

    // ------------------------------------------------------------------------
    /** Special constructor that adds n bytes of binary data.
     */
    Message1(int index, T1 &t1, const void *p, size_t n) : Message(MT, index)
    {
        size_t n1 = getSize(t1);
        allocate(n1+n);
        add(t1);
        add(p, n);
    }   // Message1

    // ------------------------------------------------------------------------
    Message1(const void *buffer, int n, T1 *t1) : Message(MT, buffer, n)
    {
        m_needs_destroy=false;
        get(t1);
    }   // Message1
    // ------------------------------------------------------------------------
    Message1(const void *buffer, int n, T1 *t1, void **p) : Message(MT, buffer, n)
    {
        m_needs_destroy=false;
        get(t1);
        *p = get();
    }   // Message1
};   // class Message1

// ============================================================================
/** Convenient template class that takes 2 argumentS. Using a template has
 *  the advantage that type checking can be done at compile time.
 */
template <Message::MessageType MT, typename T1, typename T2>
class Message2 : public Message
{

public:
    Message2(int index, const T1 &t1, const T2 &t2) : Message(MT, index)
    {
        size_t n1 = getSize(t1);
        size_t n2 = getSize(t2);
        allocate(n1+n2);
        add(t1);
        add(t2);
    }   // Message2

    // ------------------------------------------------------------------------
    /** Special constructor that adds n bytes of binary data.
     */
    Message2(int index, const T1 &t1, const T2 &t2, const void *p, size_t n) 
        : Message(MT, index)
    {
        size_t n1 = getSize(t1);
        size_t n2 = getSize(t2);
        allocate(n1+n2+n);
        add(t1);
        add(t2);
        add(p,   n);
    }   // Message2
    // ------------------------------------------------------------------------
    Message2(const void *buffer, int n, T1 *t1, T2 *t2) : Message(MT, buffer, n)
    {
        assert(getType()==MT);
        m_needs_destroy=false;
        get(t1);
        get(t2);
    }
    // ------------------------------------------------------------------------
    Message2(const void *buffer, int n, T1 *t1, T2 *t2, void **data) 
           : Message(MT, buffer, n)
    {
        assert(getType()==MT);
        m_needs_destroy=false;
        get(t1);
        get(t2);
        *data = get();
    }
};   // class Message2

// ============================================================================
/** Convenient template class that takes 2 argumentS. Using a template has
 *  the advantage that type checking can be done at compile time.
 */
template <Message::MessageType MT, typename T1, typename T2, typename T3>
class Message3 : public Message
{

public:
    Message3(int index,    const T1 &t1, 
             const T2 &t2, const T3 &t3) : Message(MT, index)
    {
        size_t n1 = getSize(t1);
        size_t n2 = getSize(t2);
        size_t n3 = getSize(t3);
        allocate(n1+n2+n3);
        add(t1);
        add(t2);
        add(t3);
    }   // Message3

    // ------------------------------------------------------------------------
    Message3(char *buffer, int n, T1 *t1, T2 *t2, T3 *t3) : Message(MT, buffer, n)
    {
        m_needs_destroy=false;
        get(t1);
        get(t2);
        get(t3);
    }
};   // class Message3

// ============================================================================

typedef Message2<Message::MSG_FOPEN,        std::string, std::string> Message_fopen;
typedef Message2<Message::MSG_FOPEN64,      std::string, std::string> Message_fopen64;
typedef Message2<Message::MSG_FSEEK,        long,        int        > Message_fseek_long;
typedef Message2<Message::MSG_FSEEKO,       off_t,       int        > Message_fseek_off_t;
typedef Message2<Message::MSG_FSEEKO64,     off64_t,     int        > Message_fseek_off64_t;
typedef Message2<Message::MSG_FSEEK_ANSWER, int,         int        > Message_fseek_answer;
typedef Message0<Message::MSG_FTELL                                 > Message_ftell;
typedef Message0<Message::MSG_FTELLO                                > Message_ftello;
typedef Message0<Message::MSG_FTELLO64                              > Message_ftello64;
typedef Message0<Message::MSG_FERROR                                > Message_ferror;
typedef Message2<Message::MSG_FWRITE,       size_t,      size_t     > Message_fwrite;
typedef Message2<Message::MSG_FREAD,        size_t,      size_t     > Message_fread;
typedef Message0<Message::MSG_FREAD_ANSWER                          > Message_fread_answer;
typedef Message0<Message::MSG_FCLOSE                                > Message_fclose;
typedef Message0<Message::MSG_FEOF                                  > Message_feof;
typedef Message1<Message::MSG_FGETS,        int                     > Message_fgets;
typedef Message3<Message::MSG_OPEN,         std::string, int, mode_t> Message_open;
typedef Message3<Message::MSG_OPEN64,       std::string, int, mode_t> Message_open64;
typedef Message0<Message::MSG___XSTAT                               > Message_stat;
typedef Message0<Message::MSG_QUIT                                  > Message_quit;
typedef Message2<Message::MSG_LSEEK,        off_t,       int        > Message_lseek_off_t;
typedef Message2<Message::MSG_LSEEK64,      off64_t,     int        > Message_lseek_off64_t;
typedef Message1<Message::MSG_WRITE,        size_t                  > Message_write;
typedef Message1<Message::MSG_READ,         size_t                  > Message_read;
typedef Message0<Message::MSG_CLOSE                                 > Message_close;
typedef Message2<Message::MSG_RENAME,       std::string, std::string> Message_rename;

#endif

