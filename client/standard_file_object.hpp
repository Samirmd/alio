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

#ifndef HEADER_STANDARD_FILE_OBJECT_HPP
#define HEADER_STANDARD_FILE_OBJECT_HPP

#include "client/base_file_object.hpp"
#include "client/config.hpp"
#include "tools/os.hpp"

#include <string>

namespace ALIO
{
class StandardFileObject : public BaseFileObject
{
protected:
    /** The original stream pointer in case of stream objects. */
    FILE *m_file;

    /** The original file descriptor. */
    int  m_filedes;

public:

    /** No static init function needed. */
    static int init() { return 0; }
    // ------------------------------------------------------------------------
    /** No static atExit function needed. */
    static int atExit() { return 0; }
    // ------------------------------------------------------------------------
    StandardFileObject(const XMLNode *info) : BaseFileObject(info)
    {
        m_file     = NULL; 
        m_filedes  = -1;
    };   // StandardFileObject

    // ------------------------------------------------------------------------
    virtual ~StandardFileObject()
    {
    };   // ~StandardFileObject

    // ------------------------------------------------------------------------
    virtual FILE*  fopen(const char *mode)
    {
        m_file=OS::fopen(getFilename().c_str(), mode);
        if(!m_file)
            return NULL;
        // We need to save the original filedes, in case that the
        // program uses fileno to get the file descriptor.
        m_filedes = OS::fileno(m_file);
        return (FILE*)this;
    }   // fopen

    // ------------------------------------------------------------------------
    virtual FILE*  fopen64(const char *mode)
    {
        m_file=OS::fopen64(getFilename().c_str(), mode);
        if(!m_file)
            return NULL;
        // We need to save the original filedes, in case that the
        // program uses fileno to get the file descriptor.
        m_filedes = OS::fileno(m_file);
        return (FILE*)this;
    }   // fopen

    // ------------------------------------------------------------------------
    virtual int setvbuf(char *buf, int mode, size_t size)
    {
        return OS::setvbuf(m_file, buf, mode, size);
    }   // setvbuf
    // ------------------------------------------------------------------------
    virtual int fseek(long offset, int whence)
    {
        return OS::fseek(m_file, offset, whence);
    }   // fseeko
    // ------------------------------------------------------------------------
    virtual int fseeko(off_t offset, int whence)
    {
        return OS::fseeko(m_file, offset, whence);
    }   // fseek
    // ------------------------------------------------------------------------
    virtual int fseeko64(off64_t offset, int whence)
    {
        return OS::fseeko64(m_file, offset, whence);
    }   // fseeko64
    // ------------------------------------------------------------------------
    virtual long ftell()
    {
        return OS::ftell(m_file);
    }   // ftell
    // ------------------------------------------------------------------------
    virtual off_t ftello()
    {
        return OS::ftello(m_file);
    }   // ftello
    // ------------------------------------------------------------------------
    virtual off64_t ftello64()
    {
        return OS::ftello64(m_file);
    }   // ftello64
    // ------------------------------------------------------------------------
    virtual int fflush()
    {
        return OS::fflush(m_file);
    }   // fflush
    // ------------------------------------------------------------------------
    virtual int ferror()
    {
        return OS::ferror(m_file);
    }   // ferror
    // ------------------------------------------------------------------------
    virtual int fileno()
    {
        // We don't return m_filedes (since otherwise any further access
        // would bypass alio), instead we return the index of this object
        // in the Config objects plus offset, so that this filedes is
        // recognised to be an alio filedes later
        return getIndex() + Config::get()->getMaxFiles();
    }   // fileno
    // ------------------------------------------------------------------------
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb)
    {
        return OS::fwrite(ptr, size, nmemb, m_file);
    }   // f_write

    // ------------------------------------------------------------------------
    virtual size_t fread(void *ptr,size_t size, size_t nmemb)
    {
        return OS::fread(ptr, size, nmemb, m_file);
    }   // f_read

    // ------------------------------------------------------------------------
    virtual int feof()
    {
        return OS::feof(m_file);
    }   // feof
    // ------------------------------------------------------------------------
    virtual char *fgets(char *s, int size)
    {
        return OS::fgets(s, size, m_file);
    }   // fgets
    // ------------------------------------------------------------------------
    virtual int fclose()
    {
        int error = OS::fclose(m_file);
        if(!error)
            m_file = NULL;
        return error;
    }   // fclose

    // ------------------------------------------------------------------------
    virtual int open(int flags, mode_t mode)
    {
        m_filedes = OS::open(getFilename().c_str(), flags, mode);
        return getIndex()+Config::get()->getMaxFiles();
    }   // open
    // ------------------------------------------------------------------------
    virtual int open64(int flags, mode_t mode)
    {
        m_filedes = OS::open64(getFilename().c_str(), flags, mode);
        return getIndex()+Config::get()->getMaxFiles();
    }   // open
    // ------------------------------------------------------------------------
    virtual int __xstat(int ver, struct stat *buf)
    {
        return OS::__xstat(ver, getFilename().c_str(), buf);
    }   // fstat
    // ------------------------------------------------------------------------
    virtual int __fxstat(int ver, struct stat *buf)
    {
        return OS::__fxstat(ver, m_filedes, buf);
    }   // fstat
    // ------------------------------------------------------------------------
    virtual int __fxstat64(int ver, struct stat64 *buf)
    {
        return OS::__fxstat64(ver, m_filedes, buf);
    }   // fstat
    // ------------------------------------------------------------------------
    virtual int __lxstat(int ver, struct stat *buf)
    {
        return OS::__lxstat(ver, getFilename().c_str(), buf);
    }   // fstat
    // ------------------------------------------------------------------------
    virtual off_t lseek(off_t offset, int whence)
    {
        return OS::lseek(m_filedes, offset, whence);
    }   // lseek
    // ------------------------------------------------------------------------
    virtual off64_t lseek64(off64_t offset, int whence)
    {
        return OS::lseek64(m_filedes, offset, whence);
    }   // lseek64
    // ------------------------------------------------------------------------
    virtual ssize_t write(const void *buf, size_t nbyte)
    {
        return OS::write(m_filedes, buf, nbyte);
    }   // write
    // ------------------------------------------------------------------------
    virtual ssize_t read(void *buf, size_t count)
    {
        return OS::read(m_filedes, buf, count);
    }   // read
    // ------------------------------------------------------------------------
    virtual int close()
    {
        return OS::close(m_filedes);
    }   // close
    // ------------------------------------------------------------------------
    virtual int rename(const char *newpath)
    {
        return OS::rename(getFilename().c_str(), newpath);
    }
    // ------------------------------------------------------------------------


};   // StandardFileObject

};   // namespace ALIO
#endif
