#include "client/config.hpp"
#include "client/i_file_object.hpp"

#include <unistd.h>

FILE * xfopen(const char *filename, const char *mode)
{
    AIO::Config::Config *config = AIO::Config::get();
    
    AIO::I_FileObject *fo =  config->getFileObjectFor(filename);
    return fo->fopen(mode);
}   // xfopen

// ----------------------------------------------------------------------------
size_t xfwrite(const void *ptr,size_t size, size_t nmemb, FILE *stream)
{
    AIO::I_FileObject *fo = (AIO::I_FileObject*)stream;
    return fo->fwrite(ptr, size, nmemb);
}   // xfwrite

// ----------------------------------------------------------------------------
int xfclose(FILE *fp)
{
    AIO::I_FileObject *fo = (AIO::I_FileObject*)fp;
    return fo->fclose();

}   // xfclose

// ----------------------------------------------------------------------------
