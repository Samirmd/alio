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
    virtual FILE*  fopen(const char *mode) { return NULL; }
    // ------------------------------------------------------------------------
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb) { return 0;}
    // ------------------------------------------------------------------------
    virtual int fclose() {return 0;}
    // ------------------------------------------------------------------------
    virtual off_t lseek(int fildes, off_t offset, int whence) {return 0;}
};   // NullFileObject

};   // namespace AIO
#endif
