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

#ifndef HEADER_NULL_FILE_OBJECT_HPP
#define HEADER_NULL_FILE_OBJECT_HPP

#include "client/base_file_object.hpp"
#include <string>

namespace ALIO
{
class NullFileObject : public BaseFileObject
{
public:

    NullFileObject(const XMLNode *info) : BaseFileObject(info)
    {
    };   // NullFileObject

    // ------------------------------------------------------------------------
    virtual ~NullFileObject() {};
    // ------------------------------------------------------------------------
    virtual FILE*  fopen(const char *mode) { return (FILE*)this; }
    // ------------------------------------------------------------------------
    virtual FILE*  fopen64(const char *mode) { return (FILE*)this; }
    // ------------------------------------------------------------------------
    virtual int     setvbuf(char *buf, int mode, size_t size) { return 0; }
    // ------------------------------------------------------------------------
    virtual int     fseek(long offset, int whence) { return 0; }
    // ------------------------------------------------------------------------
    virtual int     fseeko(off_t offset, int whence) { return 0; }
    // ------------------------------------------------------------------------
    virtual int     fseeko64(off64_t offset, int whence) { return 0; }
    // ------------------------------------------------------------------------
    virtual long    ftell() { return 0; }
    // ------------------------------------------------------------------------
    virtual off_t   ftello() { return 0; }
    // ------------------------------------------------------------------------
    virtual off64_t ftello64() { return 0; }
    // ------------------------------------------------------------------------
    virtual int     fflush() { return 0; }
    // ------------------------------------------------------------------------
    virtual int     ferror() { return 0; }
    // ------------------------------------------------------------------------
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb) 
    {
        return nmemb;
    }
    // ------------------------------------------------------------------------
    virtual size_t fread(void *ptr,size_t size, size_t nmemb) { return 0;}
    // ------------------------------------------------------------------------
    virtual int feof() { return 1; }
    // ------------------------------------------------------------------------
    virtual char * fgets(char *s, int size) { return 0; }
    // ------------------------------------------------------------------------
    virtual int fclose() {return 0;}
    // ------------------------------------------------------------------------
    virtual int open(int flags, mode_t mode) { return 1; }
    // ------------------------------------------------------------------------
    virtual int open64(int flags, mode_t mode) { return 1; }
    // ------------------------------------------------------------------------
    virtual int __xstat(int ver, struct stat *buf) {return 0; }
    // ------------------------------------------------------------------------
    virtual int __fxstat(int ver, struct stat *buf) {return 0; }
    // ------------------------------------------------------------------------
    virtual int __fxstat64(int ver, struct stat64 *buf) {return 0; }
    // ------------------------------------------------------------------------
    virtual int __lxstat(int ver, struct stat *buf) {return 0; }
    // ------------------------------------------------------------------------
    virtual off_t lseek(off_t offset, int whence) { return 0; }
    // ------------------------------------------------------------------------
    virtual off64_t lseek64(off64_t offset, int whence) { return 0; }
    // ------------------------------------------------------------------------
    virtual ssize_t write(const void *buf, size_t nbyte) { return nbyte; }
    // ------------------------------------------------------------------------
    virtual ssize_t read(void *buf, size_t count) { return 0; }
    // ------------------------------------------------------------------------
    virtual int close() { return 0; }
    // ------------------------------------------------------------------------
    virtual int rename(const char* newpath) {return 0;}
};   // NullFileObject

};   // namespace ALIO
#endif
