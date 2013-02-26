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
        typedef int    (*t___xstat)(int ver, const char *, struct stat *buf);
        typedef int    (*t___fxstat)(int ver, int filedes, struct stat *buf);
        typedef int    (*t___lxstat)(int ver, const char *, struct stat *buf);
        typedef off_t  (*t_lseek  )(int fildes, off_t offset, int whence);
        typedef ssize_t(*t_write  )(int fildes, const void *buf, size_t nbyte);
        typedef ssize_t(*t_read   )(int fd, void *buf, size_t count);
        typedef int    (*t_close  )(int fildes);

        typedef FILE * (*t_fopen  )(const char *FILE, const char *mode);
        typedef int    (*t_fseek  )(FILE *FP, long OFFSET, int WHENCE);
        typedef int    (*t_fprintf)(FILE *stream, const char *format, ...);
        typedef size_t (*t_fwrite )(const void *ptr,size_t size, size_t nmemb, FILE *stream);
        typedef size_t (*t_fread  )(void *ptr,size_t size, size_t nmemb, FILE *stream);
        typedef int    (*t_feof   )(FILE *stream);
        typedef char * (*t_fgets  )(char *s, int size, FILE *stream);
        typedef int    (*t_fclose )(FILE *fp);
        typedef int    (*t_rename )(const char *old, const char *newn);
    }   // extern "C"

    extern t_open    open;
    extern t___xstat __xstat;
    extern t___fxstat __fxstat;
    extern t___lxstat __lxstat;
    extern t_lseek   lseek;
    extern t_write   write;
    extern t_read    read;
    extern t_close   close;

    extern t_fopen   fopen;
    extern t_fseek   fseek;
    extern t_fprintf fprintf;
    extern t_fwrite  fwrite;
    extern t_feof    feof;
    extern t_fgets   fgets;
    extern t_fread   fread;
    extern t_fclose  fclose;

    extern t_rename  rename;
    // ---------------------------------------------------------------------
    int init();
}
}   // namespace AIO


#endif
