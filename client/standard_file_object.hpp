#ifndef HEADER_STANDARD_FILE_OBJECT_HPP
#define HEADER_STANDARD_FILE_OBJECT_HPP

#include "client/i_file_object.hpp"
#include "tools/os.hpp"

#include <string>

namespace AIO
{
class StandardFileObject : public I_FileObject
{
protected:
    /** The original filename. */
    std::string m_filename;

    /** The original stream pointer. */
    FILE *m_file;

public:

    StandardFileObject(const char *filename) : I_FileObject(filename)
    {
        m_filename = filename;
        m_file     = NULL; 
    };   // StandardFileObject

    // ------------------------------------------------------------------------
    virtual ~StandardFileObject()
    {
    };   // ~StandardFileObject

    // ------------------------------------------------------------------------
    virtual FILE*  fopen(const char *mode)
    {
        m_file=OS::fopen(m_filename.c_str(), mode);
        if(!m_file)
            return NULL;
        return (FILE*)this;
    }   // fopen

    // ------------------------------------------------------------------------
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb)
    {
        return OS::fwrite(ptr, size, nmemb, m_file);
    }   // f_write

    // ------------------------------------------------------------------------
    virtual int fclose()
    {
        int error = OS::fclose(m_file);
        if(!error)
            m_file = NULL;
        return error;
    }   // fclose

    // ------------------------------------------------------------------------
    virtual off_t lseek(int fildes, off_t offset, int whence)
    {
        return OS::lseek(fildes, offset, whence);
    }   // lseek
};   // StandardFileObject

};   // namespace AIO
#endif
