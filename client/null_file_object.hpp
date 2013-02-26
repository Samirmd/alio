#ifndef HEADER_NULL_FILE_OBJECT_HPP
#define HEADER_NULL_FILE_OBJECT_HPP

#include "client/base_file_object.hpp"
#include <string>

namespace AIO
{
class NullFileObject : public BaseFileObject
{
public:

    NullFileObject()
    {
    };   // NullFileObject

    // ------------------------------------------------------------------------
    virtual ~NullFileObject() {};
    // ------------------------------------------------------------------------
    virtual FILE*  fopen(const char *mode) { return (FILE*)this; }
    // ------------------------------------------------------------------------
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb) 
    {
        return nmemb;
    }
    // ------------------------------------------------------------------------
    virtual size_t fread(void *ptr,size_t size, size_t nmemb) { return 0;}
    // ------------------------------------------------------------------------
    virtual int feof() { return 1; }
    // ------------------------------------------------------------------------
    virtual char * fgets(char *s, int size) { return 0; }
    // ------------------------------------------------------------------------
    virtual int fclose() {return 0;}
    // ------------------------------------------------------------------------
    virtual int open(int flags, mode_t mode) { return 1; }
    // ------------------------------------------------------------------------
    virtual int __fxstat(int ver, struct stat *buf) {return 0; }
    // ------------------------------------------------------------------------
    virtual off_t lseek(off_t offset, int whence) { return 0; }
    // ------------------------------------------------------------------------
    virtual ssize_t write(const void *buf, size_t nbyte) { return nbyte; }
    // ------------------------------------------------------------------------
    virtual ssize_t read(void *buf, size_t count) { return 0; }
    // ------------------------------------------------------------------------
    virtual int close() { return 0; }
};   // NullFileObject

};   // namespace AIO
#endif
