#ifndef HEADER_FILE_OBJECT.HPP
#define HEADER_FILE_OBJECT.HPP

namespace AIO
{
class IFileObject
{
public:
    virtual FileObject() = 0;
    virtual ~FileObject() = 0;
    virtual FILE*  fopen(const char *FILE, const char *mode) = 0;
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb, FILE *stream) = 0;
    virtual int    fclose(FILE *fp) = 0;
    virtual off_t  lseek(int fildes, off_t offset, int whence) = 0;
};   // IFileObject
#endif