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

#ifndef HEADER_I_FILE_OBJECT_HPP
#define HEADER_I_FILE_OBJECT_HPP


#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>


#ifndef WIN32
#  include <unistd.h>
#endif

namespace ALIO
{

class XMLNode;

class I_FileObject
{
public:

    I_FileObject(const XMLNode *info)
    {
    };
    // ------------------------------------------------------------------------
    virtual ~I_FileObject() {}
    // ------------------------------------------------------------------------
    virtual void setFilename(const std::string &filename) = 0;
    // ------------------------------------------------------------------------
    virtual const std::string &getFilename() const = 0;
    // ------------------------------------------------------------------------
    /** Sets the index of this object in config's m_file_object array. */
    virtual void setIndex(int index) = 0;
    // ------------------------------------------------------------------------
    /** Returns the index of this object in config's m_file_object array. */
    virtual int getIndex() const = 0;
    // ------------------------------------------------------------------------

    virtual FILE*   fopen(const char *mode) = 0;
    virtual FILE*   fopen64(const char *mode) = 0;
    virtual int     setvbuf(char *buf, int mode, size_t size) = 0;
    virtual int     fseek(long offset, int whence) = 0;
    virtual int     fseeko(off_t offset, int whence) = 0;
    virtual int     fseeko64(off64_t offset, int whence) = 0;
    virtual long    ftell() = 0;
    virtual off_t   ftello() = 0;
    virtual off64_t ftello64() = 0;
    virtual int     fflush() = 0;
    virtual int     ferror() = 0;
    virtual int     fileno() = 0;
    virtual size_t  fwrite(const void *ptr, size_t size, size_t nmemb) = 0;
    virtual size_t  fread(void *ptr,size_t size, size_t nmemb) = 0;
    virtual int     feof() = 0;
    virtual char *  fgets(char *s, int size) = 0;
    virtual int     fclose() = 0;

    virtual int     open(int flags, mode_t mode) = 0;
    virtual int     open64(int flags, mode_t mode) = 0;
    virtual int     __xstat(int ver, struct stat *buf) = 0;
    virtual int     __fxstat(int ver, struct stat *buf) = 0;
    virtual int     __fxstat64(int ver, struct stat64 *buf) = 0;
    virtual int     __lxstat(int ver, struct stat *buf) = 0;
    virtual off_t   lseek(off_t offset, int whence) = 0;
    virtual off64_t lseek64(off64_t offset, int whence) = 0;
    virtual ssize_t write(const void *buf, size_t nbyte) = 0;
    virtual ssize_t read(void *buf, size_t count) = 0;
    virtual int     close() = 0;
    virtual int     rename(const char *newpath) = 0;
};   // IFileObject

}   // namespace ALIO
#endif
