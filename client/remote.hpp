#ifndef HEADER_REMOTE_HPP
#define HEADER_REMOTE_HPP

#include "client/base_file_object.hpp"
#include "tools/os.hpp"

#include <string>

namespace AIO
{
class Remote : public BaseFileObject
{

public:

    Remote();
    virtual ~Remote();
    virtual FILE*  fopen(const char *mode);
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb);
    virtual int fclose();
    virtual off_t lseek(int fildes, off_t offset, int whence);

};   // Remote

};   // namespace AIO
#endif
