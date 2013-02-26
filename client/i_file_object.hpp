#ifndef HEADER_I_FILE_OBJECT_HPP
#define HEADER_I_FILE_OBJECT_HPP

#include <stdio.h>
#include <string>
#include <sys/types.h>

#ifndef WIN32
#  include <unistd.h>
#endif


namespace AIO
{

class I_FileObject
{
public:
    /** This stores the index of this object in config's m_file_object
     *  array. This allows us for a quick lookup of this item in config,
     *  and to distringuish it from non-AIO managed file objects.
     */
    int m_index;

    I_FileObject() 
    {
    };
    // ------------------------------------------------------------------------
    virtual ~I_FileObject() {};
    // ------------------------------------------------------------------------
    virtual void setFilename(const std::string &filename) = 0;
    // ------------------------------------------------------------------------
    virtual const std::string &getFilename() const = 0;
    // ------------------------------------------------------------------------
    /** Sets the index of this object in config's m_file_object array. */
    void setIndex(int index) { m_index = index; }
    // ------------------------------------------------------------------------
    /** Returns the index of this object in config's m_file_object array. */
    int getIndex() const {return m_index; }
    // ------------------------------------------------------------------------

    virtual FILE*  fopen(const char *mode) = 0;
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb) = 0;
    virtual size_t fread(void *ptr,size_t size, size_t nmemb) = 0;
    virtual off_t  lseek(int fildes, off_t offset, int whence) = 0;
    virtual int    feof() = 0;
    virtual char * fgets(char *s, int size) = 0;
    virtual int    fclose() = 0;
};   // IFileObject

}   // namespace AIO
#endif
