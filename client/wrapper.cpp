#include "tools/os.hpp"
#include "client/config.hpp"
#include "client/i_file_object.hpp"

#ifndef WIN32
#  include <unistd.h>
#endif

FILE * fopen(const char *filename, const char *mode)
{
    AIO::Config *config = AIO::Config::get();
    if(!config)
    {
        return AIO::OS::fopen(filename, mode);
    }

    AIO::I_FileObject *fo =  config->createFileObject(filename);
    if(!fo)
        return AIO::OS::fopen(filename, mode);
    return fo->fopen(mode);
}   // xfopen

// ----------------------------------------------------------------------------
size_t fwrite(const void *ptr,size_t size, size_t nmemb, FILE *stream)
{
    AIO::Config *config = AIO::Config::get();
    AIO::I_FileObject *fo = config->getFileObject(stream);
    if(!fo)
        return AIO::OS::fwrite(ptr, size, nmemb, stream);

    return fo->fwrite(ptr, size, nmemb);
}   // xfwrite

// ----------------------------------------------------------------------------
size_t fread(void *ptr,size_t size, size_t nmemb, FILE *stream)
{
    AIO::Config *config = AIO::Config::get();
    if(!config)
        return AIO::OS::fread(ptr, size, nmemb, stream);
    AIO::I_FileObject *fo = config->getFileObject(stream);
    if(!fo)
        return AIO::OS::fread(ptr, size, nmemb, stream);
    return fo->fread(ptr, size, nmemb);
}   // xfwrite

// ----------------------------------------------------------------------------
int feof(FILE *stream) __THROW __wur
{
    AIO::Config *config = AIO::Config::get();
    if(!config)
        return AIO::OS::feof(stream);
    AIO::I_FileObject *fo = config->getFileObject(stream);
    if(!fo)
        return AIO::OS::feof(stream);

    return fo->feof();
}   // xfeof

// ----------------------------------------------------------------------------
char *fgets(char *s, int size, FILE *stream)
{
    AIO::Config *config = AIO::Config::get();
    AIO::I_FileObject *fo = config->getFileObject(stream);
    if(!fo)
        return AIO::OS::fgets(s, size, stream);
    return fo->fgets(s, size);
}   // xfgets

// ----------------------------------------------------------------------------
int fclose(FILE *fp)
{
    AIO::Config *config = AIO::Config::get();
    if(!config)
        return AIO::OS::fclose(fp);
    AIO::I_FileObject *fo = config->getFileObject(fp);
    if(!fo)
        return AIO::OS::fclose(fp);
    return fo->fclose();

}   // xfclose

// ----------------------------------------------------------------------------
