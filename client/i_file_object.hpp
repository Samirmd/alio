#ifndef HEADER_I_FILE_OBJECT_HPP
#define HEADER_I_FILE_OBJECT_HPP


#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>


#ifndef WIN32
#  include <unistd.h>
#endif

namespace AIO
{

class I_FileObject
{
public:

    I_FileObject() 
    {
    };
    // ------------------------------------------------------------------------
    virtual ~I_FileObject() {}
    // ------------------------------------------------------------------------
    virtual void setFilename(const std::string &filename) = 0;
    // ------------------------------------------------------------------------
    virtual const std::string &getFilename() const = 0;
    // ------------------------------------------------------------------------
    /** Sets the index of this object in config's m_file_object array. */
    virtual void setIndex(int index) = 0;
    // ------------------------------------------------------------------------
    /** Returns the index of this object in config's m_file_object array. */
    virtual int getIndex() const = 0;
    // ------------------------------------------------------------------------

    virtual FILE*   fopen(const char *mode) = 0;
    virtual size_t  fwrite(const void *ptr, size_t size, size_t nmemb) = 0;
    virtual size_t  fread(void *ptr,size_t size, size_t nmemb) = 0;
    virtual int     feof() = 0;
    virtual char *  fgets(char *s, int size) = 0;
    virtual int     fclose() = 0;

    virtual int     open(int flags, mode_t mode) = 0;
    virtual int     __xstat(int ver, struct stat *buf) = 0;
    virtual int     __fxstat(int ver, struct stat *buf) = 0;
    virtual int     __lxstat(int ver, struct stat *buf) = 0;
    virtual off_t   lseek(off_t offset, int whence) = 0;
    virtual ssize_t write(const void *buf, size_t nbyte) = 0;
    virtual ssize_t read(void *buf, size_t count) = 0;
    virtual int     close() = 0;
    virtual int     rename(const char *newpath) = 0;
};   // IFileObject

}   // namespace AIO
#endif
