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
        MSG_FWRITE, 
        MSG_FREAD,
        MSG_FEOF,
        MSG_FGETS,
        MSG_FCLOSE,
        MSG_QUIT
    } MessageType ;

private:
    char        *m_data;
    MessageType  m_type;
    int          m_data_size;
    unsigned int m_pos; // simple stack counter for constructing packet data
    bool         m_needs_destroy;  // only received messages need to be destroyed

public:
    void         addInt(int data);
    void         addShort(short data);
    void         addString(const std::string &data); 
    void         addUInt(unsigned int data)      { addInt(*(int*)&data);  }
    void         addFloat(const float data);    
    void         addBool(bool data)              { addChar(data?1:0);     }
    void         addChar(char data)              { addCharArray((char*)&data,1);}
    void         addCharArray(const char *c, unsigned int n) 
                                                 { assert((int)(m_pos+n)<=m_data_size);
                                                   memcpy(m_data+m_pos,c,n);
                                                   m_pos+=n;              }
#ifndef WIN32          // on windows size_t is unsigned int
    void         add(size_t data)                { addInt((int)data);     }
#endif
    void         addIntArray(int *d, unsigned int n) 
                                                 { for(unsigned int i=0; 
                                                       i<n; i++)
                                                       addInt(d[i]);      }
    int          getInt(); 
    short        getShort();
    float        getFloat();
    std::string  getString();
    char*        getCurrentBuffer()             {return m_data+m_pos; }
    void         getCharArray(char *c, int n=1) {memcpy(c,m_data+m_pos,n);
                                                  m_pos+=n;
                                                  return;                 }
    static int   getIntLength()             { return sizeof(int);     }
    static int   getUIntLength()            { return sizeof(int);     }
    static int   getShortLength()           { return sizeof(short);   }
    static int   getCharLength()            { return sizeof(char);    }
    static int   getBoolLength()            { return sizeof(char);    }
    static int   getFloatLength()           { return sizeof(float);   }
    static int   getStringLength(const std::string& s) { return s.size()+1;}
#ifndef WIN32
    static int   getSizeTLength()           { return sizeof(int);     }
#endif

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

