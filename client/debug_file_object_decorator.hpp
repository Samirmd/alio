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

#ifndef HEADER_DEBUG_FILE_OBJECT_DECORATOR_HPP
#define HEADER_DEBUG_FILE_OBJECT_DECORATOR_HPP

#include "client/i_file_object_decorator.hpp"
#include "tools/os.hpp"
#include "xml/xml_node.hpp"

#include <errno.h>
#include <cstdio>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

namespace ALIO
{
class XMLNode;

class DebugFileObjectDecorator: public I_FileObjectDecorator
{
private:
    /** Give name to the bit flags. */
    enum
    {
        DO_NONE = 0x0000,
        DO_OPEN = 0x0001,
        DO_CLOSE = 0x0002,
        DO_READ = 0x0004,
        DO_WRITE = 0x0008,
        DO_SEEK = 0x0010,
        DO_STAT = 0x0020,
        DO_MISC = 0x0040,
        DO_ALL = 0xffff
    };
    /** Bit flag to indicate which output is enabled. */
    int m_enable;
    /** Bit flag to indicate which output should be verbose. */
    int m_verbose;
    /** Start time, used to print a timestamp. */
    static double m_start_time;

    // ------------------------------------------------------------------------
    /** Sets the flags in m_enable and m_verbose depending on the data in the
     *  xml node.
     *  \param node The XML node.
     *  \param node_name Name to check for (nothing happens if the xml node
     *         has a different name).
     *  \param flag Which bit to set.
     */
    void setFlags(const XMLNode *node, const std::string &node_name, int flag)
    {
        if (node->getName() == node_name)
        {
            std::string s;
            node->get("mode", &s);
            if (s == "enable")
                m_enable |= flag;
            else if (s == "verbose")
            {
                m_enable |= flag;
                m_verbose |= flag;
            }
        }   // is node_name
    }
    // ------------------------------------------------------------------------
    /** A simple log replacement to make it easier to get all output to the
     *  same place.
     */
    void log(const char *format, ...)
    {
        va_list args;
        va_start(args, format);
        vfprintf(stdout, format, args);
        va_end(args);
    }   // log
    // ------------------------------------------------------------------------
    void logBinary(const void *p, long n)
    {
        OS::fwrite(p, 1, n, stdout);
    }   // logBinary

public:
    // ------------------------------------------------------------------------
    /** Static initialisation function, which sets the start time so that
     *  all other times can be printed in seconds since start. 
     */
    static int init()
    {
        m_start_time = 0.0; // now getSecondsSinceStart will return current time
        m_start_time = getSecondsSinceStart();
        return 0;
    }   // init
    // ------------------------------------------------------------------------
    /** No static atExit function needed. */
    static int atExit()
    {
        return 0;
    }

    // ------------------------------------------------------------------------
    DebugFileObjectDecorator(I_FileObject *parent, const XMLNode *info) :
            I_FileObjectDecorator(parent, info)
    {
        m_enable = DO_ALL;
        m_verbose = DO_NONE;
        std::string s = "enable";
        info->get("default", &s);
        if (s == "disable")
        {
            m_enable = DO_NONE;
        }
        else if (s == "enable")
        {
            m_enable = DO_ALL;
        }
        else
        {
            log("Invalid default '%s' - enabling all.", s.c_str());
        }
        // Now read the special additional flags
        for (unsigned int i = 0; i < info->getNumNodes(); i++)
        {
            const XMLNode *node = info->getNode(i);
            setFlags(node, "read", DO_READ);
            setFlags(node, "write", DO_WRITE);
        }
    }   //

    // ------------------------------------------------------------------------
    virtual ~DebugFileObjectDecorator()
    {
    }   // DebugFileObjectDecorator

    // ------------------------------------------------------------------------
    /** Returns the time in seconds since epoch. */
    static double getSecondsSinceStart()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec + tv.tv_usec * 1.0e-6 - m_start_time;
    }   // getSecondsSinceStart

    // ------------------------------------------------------------------------
    /** Prints the filename and the time stamp.
     */
    void header()
    {
        log("%s @ %lf ", I_FileObjectDecorator::getFilename().c_str(),
                getSecondsSinceStart());
    }   // header

    // ------------------------------------------------------------------------
    /** Convenient shortcut for functions with one argument. */
    void header(const char *format)
    {
        header();
        log(format);
    }
    // ------------------------------------------------------------------------
    /** Convenient shortcut for functions with one argument. */
    template<typename T>
    void header(const char *format, T t)
    {
        header();
        log(format, t);
    }
    // ------------------------------------------------------------------------
    /** Convenient shortcut for functions with two arguments. */
    template<typename T1, typename T2>
    void header(const char *format, T1 t1, T2 t2)
    {
        header();
        log(format, t1, t2);
    }
    // ------------------------------------------------------------------------
    /** Convenient shortcut for functions with three arguments. */
    template<typename T1, typename T2, typename T3>
    void header(const char *format, T1 t1, T2 t2, T3 t3)
    {
        header();
        log(format, t1, t2, t3);
    }
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    // Now come all wrappers. Consult the man pages to find out the details
    // about those functions. The wrapper here print the header, function
    // name and parameters, before calling the original function. When the
    // original function returns, this function prints the exit core/error
    // number.
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual FILE* fopen(const char *mode)
    {
        bool enable = m_enable & DO_OPEN;
        if (enable) header("fopen(\"%s\")", mode);
        FILE *file = I_FileObjectDecorator::fopen(mode);
        if (enable)
            if (!file)
                log(" = errno(%d).\n", errno);
            else
                log(" = %lx\n", file);
        return (FILE*) this;
    }   // fopen

    // ------------------------------------------------------------------------
    virtual FILE* fopen64(const char *mode)
    {
        bool enable = m_enable & DO_OPEN;
        if (enable) header("fopen64(\"%s\")", mode);
        FILE *file = I_FileObjectDecorator::fopen64(mode);
        if (enable)
            if (!file)
                log(" = errno(%d).\n", errno);
            else
                log(" = %lx\n", file);
        return (FILE*) this;
    }   // fopen64

    // ------------------------------------------------------------------------
    virtual int setvbuf(char *buf, int mode, size_t size)
    {
        bool enable = m_enable & DO_MISC;
        if (enable) header("setvbuf(%lx, %d, %ld)", buf, mode, size);
        int result = I_FileObjectDecorator::setvbuf(buf, mode, size);
        if (enable) log(" = %d\n", result);
        return result;
    }   // setvbuf

    // ------------------------------------------------------------------------
    virtual int fseek(long offset, int whence)
    {
        bool enable = m_enable & DO_SEEK;
        if (enable) header("fseek(%ld, %d)", offset, whence);
        int result = I_FileObjectDecorator::fseek(offset, whence);
        if (enable) log(" = %d\n", result);
        return result;
    }   // fseek

    // ------------------------------------------------------------------------
    virtual int fseeko(off_t offset, int whence)
    {
        bool enable = m_enable & DO_SEEK;
        if (enable) header("fseeko(%ld, %d)", offset, whence);
        int result = I_FileObjectDecorator::fseeko(offset, whence);
        if (enable) log(" = %d\n", result);
        return result;
    }   // fseeko

    // ------------------------------------------------------------------------
    virtual int fseeko64(off64_t offset, int whence)
    {
        bool enable = m_enable & DO_SEEK;
        if (enable) header("fseeko64(%ld, %d)", offset, whence);
        int result = I_FileObjectDecorator::fseeko64(offset, whence);
        if (enable) log(" = %d\n", result);
        return result;
    }   // fseeko64

    // ------------------------------------------------------------------------
    virtual long ftell()
    {
        bool enable = m_enable & DO_MISC;
        if (enable) header("ftell()");
        long result = I_FileObjectDecorator::ftell();
        if (enable) log(" = %ld\n", result);
        return result;
    }   // ftell

    // ------------------------------------------------------------------------
    virtual off_t ftello()
    {
        bool enable = m_enable & DO_MISC;
        if (enable) header("ftello()");
        long result = I_FileObjectDecorator::ftello();
        if (enable) log(" = %ld\n", result);
        return result;
    }   // ftell

    // ------------------------------------------------------------------------
    virtual off64_t ftello64()
    {
        bool enable = m_enable & DO_MISC;
        if (enable) header("ftello64()");
        long result = I_FileObjectDecorator::ftello64();
        if (enable) log(" = %ld\n", result);
        return result;
    }   // ftello64

    // ------------------------------------------------------------------------
    virtual int fflush()
    {
        bool enable = m_enable & DO_MISC;
        if (enable) header("fflush()");
        int result = I_FileObjectDecorator::fflush();
        if (enable) log(" = %d\n", result);
        return result;
    }   // fflush

    // ------------------------------------------------------------------------
    virtual int ferror()
    {
        bool enable = m_enable & DO_MISC;
        if (enable) header("ferror()");
        int result = I_FileObjectDecorator::ferror();
        if (enable) log(" = %d\n", result);
        return result;
    }   // ferror
    // ------------------------------------------------------------------------
    virtual int fileno()
    {
        bool enable = m_enable & DO_MISC;
        if (enable) header("fileno()");
        int result = I_FileObjectDecorator::fileno();
        if (enable) log(" = %d\n", result);
        return result;
    }   // fileno

    // ------------------------------------------------------------------------
    virtual size_t fwrite(const void *ptr, size_t size, size_t nmemb)
    {
        bool enable = m_enable & DO_WRITE;
        if (enable)
        {
            if (m_verbose & DO_WRITE)
            {
                header("fwrite('");
                logBinary(ptr, size * nmemb);
                log("'@%lx, %ld, %ld)", ptr, size, nmemb);
            }
            else
                header("fwrite(%lx, %ld, %ld)", ptr, size, nmemb);
        }
        size_t n = I_FileObjectDecorator::fwrite(ptr, size, nmemb);
        if (enable) log(" = %ld.\n", n);
        return n;
    }   // fwrite

    // ------------------------------------------------------------------------
    virtual size_t fread(void *ptr, size_t size, size_t nmemb)
    {
        bool enable = m_enable & DO_READ;
        if (enable) header("fread(%lx, %ld, %ld)", ptr, size, nmemb);
        size_t n = I_FileObjectDecorator::fread(ptr, size, nmemb);
        if (enable)
        {
            if (m_verbose & DO_READ)
            {
                log(" '");
                logBinary(ptr, size * n);
                log("' = %ld\n", n);
            }
            else
                log(" = %ld\n", n);
        }
        return n;
    }   // fread

    // ------------------------------------------------------------------------
    virtual int feof()
    {
        bool enable = m_enable & DO_MISC;
        if (enable) header("feof()");
        std::fflush(stdout);
        int n = I_FileObjectDecorator::feof();
        if (enable) log(" = %d\n", n);
        return n;
    }   // feof

    // ------------------------------------------------------------------------
    virtual char * fgets(char *s, int size)
    {
        bool enable = m_enable & DO_READ;
        if (enable) header("fgets(%lx, %d)", s, size);
        char *result = I_FileObjectDecorator::fgets(s, size);
        if (enable)
        {
            if(m_verbose & DO_READ)
            {
                log(" '%s' = %ld\n", s, result);
            }
            else
                log(" = %lx\n", result);
        }
        return result;
    }   // fgets

// ------------------------------------------------------------------------
    virtual int fclose()
    {
        bool enable = m_enable & DO_CLOSE;
        if (enable) header("fclose()");
        std::fflush(stdout);
        int error = I_FileObjectDecorator::fclose();
        if (enable) log(" = %d\n", error);
        return error;
    }   // fclose

// ------------------------------------------------------------------------
    virtual int open(int flags, mode_t mode)
    {
        bool enable = m_enable & DO_OPEN;
        if (enable) header("open(%d, %d)", flags, mode);
        int filedes = I_FileObjectDecorator::open(flags, mode);
        if (enable) log(" = %d\n", filedes);
        return filedes;
    }   // open

// ------------------------------------------------------------------------
    virtual int open64(int flags, mode_t mode)
    {
        bool enable = m_enable & DO_OPEN;
        if (enable) header("open64(%d, %d)", flags, mode);
        int filedes = I_FileObjectDecorator::open64(flags, mode);
        if (enable) log(" = %d\n", filedes);
        return filedes;
    }   // open64

// ------------------------------------------------------------------------
    virtual int __xstat(int ver, struct stat *buf)
    {
        bool enable = m_enable & DO_STAT;
        if (enable) header("stat(%lx)", buf);
        std::fflush(stdout);
        int error = I_FileObjectDecorator::__xstat(ver, buf);
        if (enable) log(" = %d\n", error);
        return error;
    }   // __xstat

// ------------------------------------------------------------------------
    virtual int __fxstat(int ver, struct stat *buf)
    {
        bool enable = m_enable & DO_STAT;
        if (enable) header("fstat(%lx)", buf);
        std::fflush(stdout);
        int error = I_FileObjectDecorator::__fxstat(ver, buf);
        if (enable) log(" = %d\n", error);
        return error;
    }   // __fxstat

// ------------------------------------------------------------------------
    virtual int __fxstat64(int ver, struct stat64 *buf)
    {
        bool enable = m_enable & DO_STAT;
        if (enable) header("fstat64(%lx)", buf);
        std::fflush(stdout);
        int error = I_FileObjectDecorator::__fxstat64(ver, buf);
        if (enable) log(" = %d\n", error);
        return error;
    }   // __fxstat64

// ------------------------------------------------------------------------
    virtual int __lxstat(int ver, struct stat *buf)
    {
        bool enable = m_enable & DO_MISC;
        if(enable) header("lstat(%lx)", buf);
        std::fflush(stdout);
        int error = I_FileObjectDecorator::__lxstat(ver, buf);
        if(enable) log(" = %d\n", error);
        return error;
    }   // __lxstat

// ------------------------------------------------------------------------
    virtual off_t lseek(off_t offset, int whence)
    {
        bool enable = m_enable & DO_MISC;
        if(enable) header("lseek(%ld, %d)", offset, whence);
        int error = I_FileObjectDecorator::lseek(offset, whence);
        if(enable) log(" = %d\n", error);
        return error;
    }   // lseek

// ------------------------------------------------------------------------
    virtual off64_t lseek64(off64_t offset, int whence)
    {
        bool enable = m_enable & DO_MISC;
        if(enable) header("lseek64(%ld, %d)", offset, whence);
        std::fflush(stdout);
        int error = I_FileObjectDecorator::lseek64(offset, whence);
        if(enable) log(" = %d\n", error);
        return error;
    }   // lseek64

// ------------------------------------------------------------------------
    virtual ssize_t write(const void *buf, size_t nbyte)
    {
        bool enable = m_enable & DO_MISC;
        if(enable)
        {
            if(m_verbose & DO_WRITE)
            {
                header("write('");
                logBinary(buf, nbyte);
                log("'@%lx, %ld)", buf, nbyte);

            }
            else
                header("write(%lx, %ld)", buf, nbyte);
        }
        ssize_t result = I_FileObjectDecorator::write(buf, nbyte);
        log(" = %ld\n", result);
        return result;
    }   // write

// ------------------------------------------------------------------------
    virtual ssize_t read(void *buf, size_t count)
    {
        bool enable = m_enable & DO_MISC;
        if(enable) header("read(%lx, %ld)", buf, count);
        ssize_t result = I_FileObjectDecorator::read(buf, count);
        if(enable)
        {
            if(m_verbose & DO_READ)
            {
                log(" '");
                logBinary(buf, count);
                log("' = %ld\n", result);
            }
            else
                log(" = %ld\n", result);

        }
        return result;
    }   // read

// ------------------------------------------------------------------------
    virtual int close()
    {
        bool enable = m_enable & DO_CLOSE;
        if(enable) header("close()");
        int result = I_FileObjectDecorator::close();
        if(enable) log(" = %d\n", result);
        return result;
    }   // close

// ------------------------------------------------------------------------
    virtual int rename(const char *newpath)
    {
        bool enable = m_enable & DO_MISC;
        if(enable) header("rename(%s)", newpath);
        int result = I_FileObjectDecorator::rename(newpath);
        if(enable) log(" = %d\n", result);
        return result;
    }

// ------------------------------------------------------------------------

};
// IFileObject

}// namespace ALIO
#endif
