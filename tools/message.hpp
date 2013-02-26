#ifndef HEADER_MESSAGE_HPP
#define HEADER_MESSAGE_HPP

#include <cstring>
#include <string>
#include <assert.h>

using std::memcpy;

class Message
{ 
public:
    typedef enum
    {
        MSG_FOPEN,
        MSG_FOPEN64,
        MSG_FSEEK,
        MSG_FSEEKO,
        MSG_FSEEKO64,
        MSG_FTELL,
        MSG_FTELLO,
        MSG_FTELLO64,
        MSG_FERROR,
        MSG_FWRITE, 
        MSG_FREAD,
        MSG_FEOF,
        MSG_FGETS,
        MSG_FCLOSE,
        MSG_OPEN,
        MSG_OPEN64,
        MSG___XSTAT,
        MSG___FXSTAT,
        MSG___FXSTAT64,
        MSG___LXSTAT,
        MSG_LSEEK,
        MSG_LSEEK64,
        MSG_WRITE,
        MSG_READ,
        MSG_CLOSE,
        MSG_RENAME,
        MSG_QUIT
    } MessageType ;

private:
    char        *m_data;
    MessageType  m_type;
    int          m_data_size;
    unsigned int m_pos; // simple stack counter for constructing packet data
    bool         m_needs_destroy;  // only received messages need to be destroyed

public:

    /** Template add function, that adds one element of the given type 
     * to the message. */
    template <typename TYPE>
    void add(TYPE data)
    {  
        assert((int)(m_pos + sizeof(TYPE)) <= m_data_size);
        memcpy(m_data+m_pos, &data, sizeof(TYPE)); 
        m_pos += sizeof(TYPE);
    };
    // ------------------------------------------------------------------------
    /** Specialisation for null-terminated character arrays. */
    void add(const char *c) 
    {
        int n=strlen(c);
        assert((int)(m_pos+n)<=m_data_size);
        memcpy(m_data+m_pos, c, n);
        m_pos+=n;
    }
    // ------------------------------------------------------------------------
    /** Specialisation for arbitrary binary data of a given length. */
    void add(const void *p, off_t n)
    {
        assert((int)(m_pos+n)<=m_data_size);
        memcpy(m_data+m_pos, p, n);
        m_pos+=n;
    }

    // ------------------------------------------------------------------------
    /** Specialisation for std::strings. */
    void add(const std::string &data)
    { 
        int len = data.size()+1;  // copy 0 end byte
        assert((int)(m_pos+len) <=m_data_size);
        memcpy (m_data+m_pos, data.c_str(), len);
        m_pos += len;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    /** Template class to extract an arbitrary data type from a message. */
    template <class TYPE>
    void get(TYPE &data)
    {
        memcpy(&data, m_data+m_pos, sizeof(TYPE));
        m_pos += sizeof(TYPE);
    }
    // ------------------------------------------------------------------------
    /** Specialisation for std::strings. */
    void get(std::string &name)
    {
        char *str = m_data + m_pos;
        int len   = strlen(str)+1;
        name = m_data+m_pos;
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

public:
                 Message(MessageType m);
                 Message(char *buffer, int len);
                ~Message();
    void         clear();
    void         allocate(int size);
    // ------------------------------------------------------------------------
    /** Returns the message type. */
    MessageType  getType() const   { return m_type; }
    // ------------------------------------------------------------------------
    /** Returns the data size - excluding the 1 byte type. */
    int          getLen() const { return m_data_size; }
    // ------------------------------------------------------------------------
    /** Returns a pointer to the actual data - excluding the 1 byte type. */
    char*        getData() {return m_data; }

};   // Message


#endif

