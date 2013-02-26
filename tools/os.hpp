#ifndef HEADER_OS_HPP
#define HEADER_OS_HPP

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace AIO {
namespace OS {
    extern "C"
    {
#ifdef WIN32
        typedef int    (*t_open)(const char *pathname, int flags);
#else
        typedef int    (*t_open)(const char *pathname, int flags, mode_t mode);
#endif
        typedef size_t (*t_write  )(int fildes, const void *buf, size_t nbyte);
        typedef int    (*t_close  )(int fildes);

        typedef FILE * (*t_fopen  )(const char *FILE, const char *mode);
        typedef int    (*t_fprintf)(FILE *stream, const char *format, ...);
        typedef size_t (*t_fwrite )(const void *ptr,size_t size, size_t nmemb, FILE *stream);
        typedef off_t  (*t_lseek  )(int fildes, off_t offset, int whence);
        typedef int    (*t_fclose )(FILE *fp);
    }   // extern "C"

    extern t_open    open;
    extern t_write   write;
    extern t_close   close;

    extern t_fopen   fopen;
    extern t_fprintf fprintf;
    extern t_fwrite  fwrite;
    extern t_lseek   lseek;
    extern t_fclose  fclose;
    // ---------------------------------------------------------------------
    int init();
}
}   // namespace AIO


#endif
