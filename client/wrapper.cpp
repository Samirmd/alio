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
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;
    if(!config || !(fo = config->createFileObject(filename)))
    {
        return ALIO::OS::fopen(filename, mode);
    }

    return fo->fopen(mode);
}   // fopen

// ----------------------------------------------------------------------------
FILE * fopen64(const char *filename, const char *mode)
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;
    if(!config || !(fo = config->createFileObject(filename)))
    {
        // It can happen that this wrapper is loaded and fopen64 is called
        // but ALIO::OS is not initialised.
        if(!ALIO::OS::fopen64)
        {
            ALIO::OS::t_fopen fc = (ALIO::OS::t_fopen)dlsym(RTLD_NEXT, "fopen64");
            return fc(filename, mode);
            
        }
        return ALIO::OS::fopen64(filename, mode);
    }

    return fo->fopen64(mode);
}   // fopen

// ------------------------------------------------------------------------
int setvbuf(FILE *stream, char *buf, int mode, size_t size) __THROW
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->getFileObject(stream)))
        return ALIO::OS::setvbuf(stream, buf, mode, size);

    return fo->setvbuf(buf, mode, size);
}   // setvbuf

// ------------------------------------------------------------------------
int fseek(FILE *stream, long offset, int whence)
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->getFileObject(stream)))
        return ALIO::OS::fseek(stream, offset, whence);

    return fo->fseek(offset, whence);
}   // fseek

// ------------------------------------------------------------------------
int fseeko(FILE *stream, off_t offset, int whence)
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->getFileObject(stream)))
        return ALIO::OS::fseeko(stream, offset, whence);

    return fo->fseeko(offset, whence);
}   // fseeko

// ------------------------------------------------------------------------
int fseeko64(FILE *stream, off64_t offset, int whence)
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->getFileObject(stream)))
        return ALIO::OS::fseeko64(stream, offset, whence);

    return fo->fseeko64(offset, whence);
}   // fseeko64

// ------------------------------------------------------------------------
long ftell(FILE *stream)
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->getFileObject(stream)))
        return ALIO::OS::ftell(stream);

    return fo->ftell();
}   // ftell

// ------------------------------------------------------------------------
off_t ftello(FILE *stream)
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->getFileObject(stream)))
        return ALIO::OS::ftello(stream);

    return fo->ftello();
}   // ftello

// ------------------------------------------------------------------------
off64_t ftello64(FILE *stream)
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->getFileObject(stream)))
        return ALIO::OS::ftello64(stream);

    return fo->ftello64();
}   // ftello64

// ------------------------------------------------------------------------
int fflush(FILE *stream)
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->getFileObject(stream)))
        return ALIO::OS::fflush(stream);

    return fo->fflush();
}   // fflush

// ------------------------------------------------------------------------
int ferror(FILE *stream) __THROW
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->getFileObject(stream)))
        return ALIO::OS::ferror(stream);

    return fo->ferror();
}   // ferror

// ----------------------------------------------------------------------------
size_t fwrite(const void *ptr,size_t size, size_t nmemb, FILE *stream)
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;
    if(!config || !(fo = config->getFileObject(stream)))
        return ALIO::OS::fwrite(ptr, size, nmemb, stream);

    return fo->fwrite(ptr, size, nmemb);
}   // fwrite

// ----------------------------------------------------------------------------
size_t fread(void *ptr,size_t size, size_t nmemb, FILE *stream)
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo = config->getFileObject(stream)))
        return ALIO::OS::fread(ptr, size, nmemb, stream);

    return fo->fread(ptr, size, nmemb);
}   // fwrite

// ----------------------------------------------------------------------------
int feof(FILE *stream) __THROW __wur
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo = config->getFileObject(stream)))
        return ALIO::OS::feof(stream);

    return fo->feof();
}   // feof

// ----------------------------------------------------------------------------
char *fgets(char *s, int size, FILE *stream)
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->getFileObject(stream)))
        return ALIO::OS::fgets(s, size, stream);

    return fo->fgets(s, size);
}   // fgets

// ----------------------------------------------------------------------------
int fclose(FILE *fp)
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo = config->getFileObject(fp)))
    {
        // It can happen that this wrapper is loaded and fclose is called
        // but ALIO::OS is not initialised (I'd guess when closing this 
        // .so ?). So in this case we have to lookup fclose :(
        if(!ALIO::OS::fclose)
        {
            ALIO::OS::t_fclose fc = (ALIO::OS::t_fclose)dlsym(RTLD_NEXT, "fclose");
            return fc(fp);
            
        }
        return ALIO::OS::fclose(fp);
    }

    return fo->fclose();

}   // fclose

// ----------------------------------------------------------------------------
int open(const char *pathname, int flags, ...)
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

	if (flags & O_CREAT) 
    {
        va_list arguments;
		va_start(arguments, flags);
		mode_t mode = va_arg(arguments, mode_t);
        if(!config || !(fo=config->createFileObject(pathname)))
            return ALIO::OS::open(pathname, flags, mode);
        return fo->open(flags, mode);
    }

    int mode=0;
    if(!config || !(fo=config->createFileObject(pathname)))
        return ALIO::OS::open(pathname, flags, mode);

    return fo->open(flags, mode);

}   // open

// ----------------------------------------------------------------------------
int open64(const char *pathname, int flags, ...)
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

	if (flags & O_CREAT) 
    {
        va_list arguments;
		va_start(arguments, flags);
		mode_t mode = va_arg(arguments, mode_t);
        if(!config || !(fo=config->createFileObject(pathname)))
            return ALIO::OS::open64(pathname, flags, mode);
        return fo->open64(flags, mode);
    }

    int mode=0;
    if(!config || !(fo=config->createFileObject(pathname)))
        return ALIO::OS::open64(pathname, flags, mode);

    return fo->open64(flags, mode);

}   // open

// ----------------------------------------------------------------------------
int __fxstat(int ver, int filedes, struct stat *buf) __THROW
{   
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;
    if(!config || !(fo =  config->getFileObject(filedes)))
    {
        // It can happen that this wrapper is loaded and fstat is called
        // but ALIO::OS is not initialised. So in this case we have to
        // lookup fxstat :(
        if(!ALIO::OS::__fxstat)
        {
            ALIO::OS::t___fxstat fc = (ALIO::OS::t___fxstat)dlsym(RTLD_NEXT, "__fxstat");
            return fc(ver, filedes, buf);
            
        }
        return ALIO::OS::__fxstat(ver, filedes, buf);
    }
    return fo->__fxstat(ver, buf);
    
}   // __fxstat
// ----------------------------------------------------------------------------
int __fxstat64(int ver, int filedes, struct stat64 *buf) __THROW
{   
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;
    if(!config || !(fo =  config->getFileObject(filedes)))
    {
        // It can happen that this wrapper is loaded and fstat is called
        // but ALIO::OS is not initialised. So in this case we have to
        // lookup fxstat :(
        if(!ALIO::OS::__fxstat64)
        {
            ALIO::OS::t___fxstat64 fc = (ALIO::OS::t___fxstat64)dlsym(RTLD_NEXT, "__fxstat64");
            return fc(ver, filedes, buf);
            
        }
        return ALIO::OS::__fxstat64(ver, filedes, buf);
    }
    return fo->__fxstat64(ver, buf);
    
}   // __fxstat64

// ----------------------------------------------------------------------------
off_t lseek(int filedes, off_t offset, int whence) __THROW
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo =  config->getFileObject(filedes)))
        return ALIO::OS::lseek(filedes, offset, whence);

    return fo->lseek(offset, whence);
}   // lseek
// ----------------------------------------------------------------------------
off64_t lseek64(int filedes, off64_t offset, int whence) __THROW
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo =  config->getFileObject(filedes)))
        return ALIO::OS::lseek64(filedes, offset, whence);

    return fo->lseek64(offset, whence);
}   // lseek
// ----------------------------------------------------------------------------
ssize_t write(int filedes, const void *buf, size_t nbyte)
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->getFileObject(filedes)))
        return ALIO::OS::write(filedes, buf, nbyte);

    return fo->write(buf, nbyte);

}   // write
// ----------------------------------------------------------------------------
ssize_t read(int filedes, void *buf, size_t count)
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->getFileObject(filedes)))
        return ALIO::OS::read(filedes, buf, count);

    return fo->read(buf, count);
}   // read
// ----------------------------------------------------------------------------
int close(int filedes)
{   
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->getFileObject(filedes)))
        return ALIO::OS::close(filedes);

    return fo->close();
}   // close
// ----------------------------------------------------------------------------
int rename(const char *oldpath, const char *newpath) __THROW
{
    ALIO::Config *config   = ALIO::Config::get();
    ALIO::I_FileObject *fo = NULL;

    if(!config || !(fo=config->createFileObject(oldpath)))
        return ALIO::OS::rename(oldpath, newpath);

    return fo->rename(newpath);

}   // rename
// ----------------------------------------------------------------------------

}
