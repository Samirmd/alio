//
//    ALIO - ALternative IO library
//    Copyright (C) 2013  Joerg Henrichs
//
//    ALIO is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    ALIO is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with ALIO.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef HEADER_OS_HPP
#define HEADER_OS_HPP

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace ALIO {
namespace OS {
    extern "C"
    {
#ifdef WIN32
        typedef int     (*t_open    )(const char *pathname, int flags);
#else
        typedef int     (*t_open    )(const char *pathname, int flags, mode_t mode);
#endif
        typedef int     (*t___xstat   )(int ver, const char *, struct stat *buf);
        typedef int     (*t___xstat64 )(int ver, const char *, struct stat64 *buf);
        typedef int     (*t___fxstat  )(int ver, int filedes, struct stat *buf);
        typedef int     (*t___fxstat64)(int ver, int filedes, struct stat64 *buf);
        typedef int     (*t___lxstat  )(int ver, const char *, struct stat *buf);
        typedef int     (*t___lxstat64)(int ver, const char *, struct stat64 *buf);
        typedef off_t   (*t_lseek   )(int fildes, off_t offset, int whence);
        typedef off64_t (*t_lseek64 )(int fildes, off64_t offset, int whence);
        typedef ssize_t (*t_write   )(int fildes, const void *buf, size_t nbyte);
        typedef ssize_t (*t_read    )(int fd, void *buf, size_t count);
        typedef int     (*t_close   )(int fildes);

        typedef FILE *  (*t_fopen   )(const char *FILE, const char *mode);
        typedef int     (*t_setvbuf )(FILE *stream, char *buf, int mode, size_t size);
        typedef int     (*t_fseek   )(FILE *stream, long offset, int whence);
        typedef int     (*t_fseeko  )(FILE *stream, off_t offset, int whence);
        typedef int     (*t_fseeko64)(FILE *stream, off64_t offset, int whence);
        typedef long    (*t_ftell   )(FILE *stream);
        typedef off_t   (*t_ftello  )(FILE *stream);
        typedef off64_t (*t_ftello64)(FILE *stream);
        typedef int     (*t_fflush  )(FILE *stream);
        typedef int     (*t_ferror  )(FILE *stream);
        typedef size_t  (*t_fwrite  )(const void *ptr,size_t size, size_t nmemb, FILE *stream);
        typedef size_t  (*t_fread   )(void *ptr,size_t size, size_t nmemb, FILE *stream);
        typedef char *  (*t_fgets   )(char *s, int size, FILE *stream);
        typedef int     (*t_feof    )(FILE *stream);
        typedef int     (*t_fclose  )(FILE *fp);

        typedef int     (*t_rename  )(const char *old, const char *newn);
    }   // extern "C"

    extern t_open     open;
    extern t_open     open64;
    extern t___xstat    __xstat;
    extern t___xstat64  __xstat64;
    extern t___fxstat   __fxstat;
    extern t___fxstat64 __fxstat64;
    extern t___lxstat   __lxstat;
    extern t___lxstat64 __lxstat64;
    extern t_lseek    lseek;
    extern t_lseek64  lseek64;
    extern t_write    write;
    extern t_read     read;
    extern t_close    close;

    extern t_fopen    fopen;
    extern t_fopen    fopen64;
    extern t_setvbuf  setvbuf;
    extern t_fseek    fseek;
    extern t_fseeko   fseeko;
    extern t_fseeko64 fseeko64;
    extern t_ftell    ftell;
    extern t_ftello   ftello;
    extern t_ftello64 ftello64;
    extern t_fflush   fflush;
    extern t_ferror   ferror;
    extern t_fwrite   fwrite;
    extern t_fread    fread;
    extern t_fgets    fgets;
    extern t_feof     feof;
    extern t_fclose   fclose;

    extern t_rename   rename;
    // ---------------------------------------------------------------------
    int init();
}
}   // namespace ALIO


#endif
