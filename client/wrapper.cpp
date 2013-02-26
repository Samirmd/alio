#include "client/config.hpp"
#include "client/i_file_object.hpp"

#ifndef WIN32
#  include <unistd.h>
#endif

FILE * xfopen(const char *filename, const char *mode)
{
    AIO::Config *config = AIO::Config::get();
    
    AIO::I_FileObject *fo =  config->createFileObject(filename);
    return fo->fopen(mode);
}   // xfopen

// ----------------------------------------------------------------------------
size_t xfwrite(const void *ptr,size_t size, size_t nmemb, FILE *stream)
{
    AIO::Config *config = AIO::Config::get();
    AIO::I_FileObject *fo = config->getFileObject(stream);
    return fo->fwrite(ptr, size, nmemb);
}   // xfwrite

// ----------------------------------------------------------------------------
int xfclose(FILE *fp)
{
    AIO::I_FileObject *fo = (AIO::I_FileObject*)fp;
    return fo->fclose();

}   // xfclose

// ----------------------------------------------------------------------------
