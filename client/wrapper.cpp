#include "tools/os.hpp"
#include "client/config.hpp"
#include "client/i_file_object.hpp"

#ifndef WIN32
#  include <unistd.h>
#  include <dlfcn.h>
#  include <stdarg.h>
#endif

extern "C"
{

FILE * fopen(const char *filename, const char *mode)
{
    AIO::Config *config   = AIO::Config::get();
    AIO::I_FileObject *fo = NULL;
    if(!config || !(fo = config->createFileObject(filename)))
    {
        return AIO::OS::fopen(filename, mode);
    }

    return fo->fopen(mode);
}   // fopen

// ----------------------------------------------------------------------------
size_t fwrite(const void *ptr,size_t size, size_t nmemb, FILE *stream)
{
    AIO::Config *config   = AIO::Config::get();
    AIO::I_FileObject *fo = NULL;
    if(!config || !(fo = config->getFileObject(stream)))
        return AIO::OS::fwrite(ptr, size, nmemb, stream);

    return fo->fwrite(ptr, size, nmemb);
}   // fwrite

// ----------------------------------------------------------------------------
size_t fread(void *ptr,size_t size, size_t nmemb, FILE *stream)
{
    AIO::Config *config   = AIO::Config::get();
    AIO::I_FileObject *fo = NULL;

    if(!config || !(fo = config->getFileObject(stream)))
        return AIO::OS::fread(ptr, size, nmemb, stream);

    return fo->fread(ptr, size, nmemb);
}   // fwrite

// ----------------------------------------------------------------------------
int feof(FILE *stream) __THROW __wur
{
    AIO::Config *config   = AIO::Config::get();
    AIO::I_FileObject *fo = NULL;

    if(!config || !(fo = config->getFileObject(stream)))
        return AIO::OS::feof(stream);

    return fo->feof();
}   // feof

// ----------------------------------------------------------------------------
char *fgets(char *s, int size, FILE *stream)
{
    AIO::Config *config   = AIO::Config::get();
    AIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->getFileObject(stream)))
        return AIO::OS::fgets(s, size, stream);

    return fo->fgets(s, size);
}   // fgets

// ----------------------------------------------------------------------------
int fclose(FILE *fp)
{
    AIO::Config *config   = AIO::Config::get();
    AIO::I_FileObject *fo = NULL;

    if(!config || !(fo = config->getFileObject(fp)))
    {
        // It can happen that this wrapper is loaded and fclose is called
        // but AIO::OS is not initialised (I'd guess when closing this 
        // .so ?). So in this case we have to lookup fclose :(
        if(!AIO::OS::fclose)
        {
            AIO::OS::t_fclose fc = (AIO::OS::t_fclose)dlsym(RTLD_NEXT, "fclose");
            return fc(fp);
            
        }
        return AIO::OS::fclose(fp);
    }

    return fo->fclose();

}   // fclose

// ----------------------------------------------------------------------------
int open(const char *pathname, int flags, ...)
{
    AIO::Config *config   = AIO::Config::get();
    AIO::I_FileObject *fo = NULL;

	if (flags & O_CREAT) 
    {
        va_list arguments;
		va_start(arguments, flags);
		mode_t mode = va_arg(arguments, mode_t);
        if(!config || !(fo=config->createFileObject(pathname)))
            return AIO::OS::open(pathname, flags, mode);
        return fo->open(flags, mode);
    }

    int mode=0;
    if(!config || !(fo=config->createFileObject(pathname)))
        return AIO::OS::open(pathname, flags, mode);

    return fo->open(flags, mode);

}   // open

// ----------------------------------------------------------------------------
int __fxstat(int ver, int filedes, struct stat *buf) __THROW
{   
    AIO::Config *config   = AIO::Config::get();
    AIO::I_FileObject *fo = NULL;
    if(!config || !(fo =  config->getFileObject(filedes)))
        return AIO::OS::__fxstat(ver, filedes, buf);

    return fo->__fxstat(ver, buf);
    
}   // __fxstat
// ----------------------------------------------------------------------------
off_t lseek(int filedes, off_t offset, int whence) __THROW
{
    AIO::Config *config   = AIO::Config::get();
    AIO::I_FileObject *fo = NULL;

    if(!config || !(fo =  config->getFileObject(filedes)))
        return AIO::OS::lseek(filedes, offset, whence);

    return fo->lseek(offset, whence);
}   // lseek
// ----------------------------------------------------------------------------
ssize_t write(int filedes, const void *buf, size_t nbyte)
{
    AIO::Config *config   = AIO::Config::get();
    AIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->getFileObject(filedes)))
        return AIO::OS::write(filedes, buf, nbyte);

    return fo->write(buf, nbyte);

}   // write
// ----------------------------------------------------------------------------
ssize_t read(int filedes, void *buf, size_t count)
{
    AIO::Config *config   = AIO::Config::get();
    AIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->getFileObject(filedes)))
        return AIO::OS::read(filedes, buf, count);

    return fo->read(buf, count);
}   // read
// ----------------------------------------------------------------------------
int close(int filedes)
{   
    AIO::Config *config   = AIO::Config::get();
    AIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->getFileObject(filedes)))
        return AIO::OS::close(filedes);

    return fo->close();
}   // close
// ----------------------------------------------------------------------------
int rename(const char *oldpath, const char *newpath) __THROW
{
    printf("Rename %s -> %s called.\n", oldpath, newpath);
    AIO::Config *config   = AIO::Config::get();
    AIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->createFileObject(oldpath)))
        return AIO::OS::rename(oldpath, newpath);

    return fo->rename(oldpath);

}   // rename
// ----------------------------------------------------------------------------

}
