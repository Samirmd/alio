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

#ifndef HEADER_TIMER_FILE_OBJECT_DECORATOR_HPP
#define HEADER_TIMER_FILE_OBJECT_DECORATOR_HPP

#include "client/i_file_object_decorator.hpp"
#include "client/timer.hpp"
#include "client/timer_data.hpp"
#include "client/timer_manager.hpp"

#include <errno.h>
#include <math.h>
#include <string>
#include <vector>

namespace ALIO
{
    /** The various events that will be timed. */
    enum TimerType { TIMER_OPEN, 
                     TIMER_CLOSE,
                     TIMER_READ,
                     TIMER_WRITE,
                     TIMER_SEEK,
                     TIMER_MISC,
                     TIMER_COUNT };


class TimerFileObjectDecorator : public I_FileObjectDecorator
{

    /** All timer info for this file. */
    TimerData *m_timer_data;

    /** True if xml should be written, false otherwise. */
    bool  m_write_xml;

    /** True if an ASCII table should be written, false otherwise. */
    bool  m_write_table;

public:
    TimerFileObjectDecorator(I_FileObject *parent, const XMLNode *info);

    // ------------------------------------------------------------------------
    virtual ~TimerFileObjectDecorator() {};
    // ------------------------------------------------------------------------
    virtual void setFilename(const std::string &filename)
    {
        I_FileObjectDecorator::setFilename(filename);
        m_timer_data = TimerManager::getTimer(TIMER_COUNT, filename, 
                                              m_write_xml, m_write_table);
    }
    // ------------------------------------------------------------------------
    virtual FILE* fopen(const char *mode)
    {
        m_timer_data->start(TIMER_OPEN);
        FILE *file = I_FileObjectDecorator::fopen(mode);
        m_timer_data->stop(TIMER_OPEN);
        return (FILE*)this;
    }
    // ------------------------------------------------------------------------
    virtual FILE* fopen64(const char *mode)
    {
        m_timer_data->start(TIMER_OPEN);
        FILE *file = I_FileObjectDecorator::fopen64(mode);
        m_timer_data->stop(TIMER_OPEN);
        return (FILE*)this;
    }
    // ------------------------------------------------------------------------
    virtual int setvbuf(char *buf, int mode, size_t size)
    {
        m_timer_data->start(TIMER_MISC);
        int result = I_FileObjectDecorator::setvbuf(buf, mode, size);
        m_timer_data->stop(TIMER_MISC);
        return result;
    }   // setvbuf

    // ------------------------------------------------------------------------
    virtual int fseek(long offset, int whence)
    {
        m_timer_data->start(TIMER_SEEK);
        int result = I_FileObjectDecorator::fseek(offset, whence);
        m_timer_data->stop(TIMER_SEEK);
        return result;
    }   // fseek

    // ------------------------------------------------------------------------
    virtual int fseeko(off_t offset, int whence)
    {
        m_timer_data->start(TIMER_SEEK);
        int result = I_FileObjectDecorator::fseeko(offset, whence);
        m_timer_data->stop(TIMER_SEEK);
        return result;
    }   // fseeko

    // ------------------------------------------------------------------------
    virtual int fseeko64(off64_t offset, int whence)
    {
        m_timer_data->start(TIMER_SEEK);
        int result = I_FileObjectDecorator::fseeko64(offset, whence);
        m_timer_data->stop(TIMER_SEEK);
        return result;
    }   // fseeko64

    // ------------------------------------------------------------------------
    virtual long ftell()
    {
        m_timer_data->start(TIMER_MISC);
        long result = I_FileObjectDecorator::ftell();
        m_timer_data->stop(TIMER_MISC);
        return result;
    }   // ftell

    // ------------------------------------------------------------------------
    virtual off_t ftello()
    {
        m_timer_data->start(TIMER_MISC);
        off_t result = I_FileObjectDecorator::ftello();
        m_timer_data->stop(TIMER_MISC);
        return result;
    }   // ftello

    // ------------------------------------------------------------------------
    virtual off64_t ftello64()
    {
        m_timer_data->start(TIMER_MISC);
        off64_t result = I_FileObjectDecorator::ftello64();
        m_timer_data->stop(TIMER_MISC);
        return result;
    }   // ftello64

    // ------------------------------------------------------------------------
    virtual int fflush()
    {
        m_timer_data->start(TIMER_MISC);
        int result = I_FileObjectDecorator::fflush();
        m_timer_data->stop(TIMER_MISC);
        return result;
    }   // fflush

    // ------------------------------------------------------------------------
    virtual int ferror()
    {
        m_timer_data->start(TIMER_MISC);
        int result = I_FileObjectDecorator::ferror();
        m_timer_data->stop(TIMER_MISC);
        return result;
    }   // ferror

    // ------------------------------------------------------------------------
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb)
    {
        m_timer_data->start(TIMER_WRITE);
        size_t n = I_FileObjectDecorator::fwrite(ptr, size, nmemb);
        m_timer_data->stop(TIMER_WRITE, n*size);
        return n;
    }
    // ------------------------------------------------------------------------
    virtual size_t fread(void *ptr,size_t size, size_t nmemb) 
    { 
        m_timer_data->start(TIMER_READ);
        size_t n = I_FileObjectDecorator::fread(ptr, size, nmemb);
        m_timer_data->stop(TIMER_READ, n*size);
        return n;
    }
    // ------------------------------------------------------------------------
    virtual int feof() 
    { 
        m_timer_data->start(TIMER_MISC);
        int n = I_FileObjectDecorator::feof();
        m_timer_data->stop(TIMER_MISC);
        return n;
    }   // feof
    // ------------------------------------------------------------------------
    virtual char * fgets(char *s, int size) 
    { 
        m_timer_data->start(TIMER_READ, size);
        char *result = I_FileObjectDecorator::fgets(s, size);
        m_timer_data->stop(TIMER_READ, result ? size-1 : 0);
        return result;
    }   // fgets
    // ------------------------------------------------------------------------
    virtual int fclose() 
    {
        m_timer_data->start(TIMER_CLOSE);
        int error = I_FileObjectDecorator::fclose(); 
        m_timer_data->stop(TIMER_CLOSE);
        return error;
    }   // fclose

    // ------------------------------------------------------------------------
    virtual int open(int flags, mode_t mode)
    {
        m_timer_data->start(TIMER_OPEN);
        int filedes = I_FileObjectDecorator::open(flags, mode);
        m_timer_data->stop(TIMER_OPEN);
        return filedes;
    }   // open
    // ------------------------------------------------------------------------
    virtual int open64(int flags, mode_t mode)
    {
        m_timer_data->start(TIMER_OPEN);
        int filedes = I_FileObjectDecorator::open64(flags, mode);
        m_timer_data->stop(TIMER_OPEN);
        return filedes;
    }   // open
    // ------------------------------------------------------------------------
    virtual int __xstat(int ver, struct stat *buf)
    {
        m_timer_data->start(TIMER_MISC);
        int error = I_FileObjectDecorator::__xstat(ver, buf);
        m_timer_data->stop(TIMER_MISC);
        return error;
    }   // fstat
    // ------------------------------------------------------------------------
    virtual int __fxstat(int ver, struct stat *buf)
    {
        m_timer_data->start(TIMER_MISC);
        int error = I_FileObjectDecorator::__fxstat(ver, buf);
        m_timer_data->stop(TIMER_MISC);
        return error;
    }   // __fxstat
    // ------------------------------------------------------------------------
    virtual int __fxstat64(int ver, struct stat64 *buf)
    {
        m_timer_data->start(TIMER_MISC);
        int error = I_FileObjectDecorator::__fxstat64(ver, buf);
        m_timer_data->stop(TIMER_MISC);
        return error;
    }   // __fxstat64
    // ------------------------------------------------------------------------
    virtual int __lxstat(int ver, struct stat *buf)
    {
        m_timer_data->start(TIMER_MISC);
        int error = I_FileObjectDecorator::__lxstat(ver, buf);
        m_timer_data->stop(TIMER_MISC);
        return error;
    }   // __lxstat
    // ------------------------------------------------------------------------
    virtual off_t lseek(off_t offset, int whence)
    {
        m_timer_data->start(TIMER_SEEK);
        int error = I_FileObjectDecorator::lseek(offset, whence);
        m_timer_data->stop(TIMER_SEEK);
        return error;
    }   // lseek
    // ------------------------------------------------------------------------
    virtual off64_t lseek64(off64_t offset, int whence)
    {
        m_timer_data->start(TIMER_SEEK);
        int error = I_FileObjectDecorator::lseek64(offset, whence);
        m_timer_data->stop(TIMER_SEEK);
        return error;
    }   // lseek64
    // ------------------------------------------------------------------------
    virtual ssize_t write(const void *buf, size_t nbyte)
    {
        m_timer_data->start(TIMER_WRITE);
        ssize_t result = I_FileObjectDecorator::write(buf, nbyte);
        m_timer_data->stop(TIMER_WRITE, result);
        return result;
    }   // write
    // ------------------------------------------------------------------------
    virtual ssize_t read(void *buf, size_t count)
    {
        m_timer_data->start(TIMER_READ);
        ssize_t result = I_FileObjectDecorator::read(buf, count);
        m_timer_data->stop(TIMER_READ, result);
        return result;
    }   // read
    // ------------------------------------------------------------------------
    virtual int close()
    {
        m_timer_data->start(TIMER_CLOSE);
        int result = I_FileObjectDecorator::close();
        m_timer_data->stop(TIMER_CLOSE);
        return result;
    }   // close
    // ------------------------------------------------------------------------
    virtual int rename(const char *newpath)
    {
        m_timer_data->start(TIMER_MISC);
        int result = I_FileObjectDecorator::rename(newpath);
        m_timer_data->stop(TIMER_MISC);
        return result;
    }
    // ------------------------------------------------------------------------

};   // TimeFileObjectDecorator

}   // namespace ALIO
#endif
