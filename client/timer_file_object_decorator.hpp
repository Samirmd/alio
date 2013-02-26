#ifndef HEADER_TIMER_FILE_OBJECT_DECORATOR_HPP
#define HEADER_TIMER_FILE_OBJECT_DECORATOR_HPP

#include "client/i_file_object_decorator.hpp"
#include "client/timer.hpp"

#include <errno.h>
#include <math.h>
#include <string>
#include <vector>

namespace AIO
{

class TimerFileObjectDecorator : public I_FileObjectDecorator
{

public:
    /** The various events that will be timed. */
    enum TimerType { TIMER_OPEN, 
                     TIMER_CLOSE,
                     TIMER_READ,
                     TIMER_WRITE,
                     TIMER_SEEK,
                     TIMER_MISC,
                     TIMER_COUNT };

private:
public:
    /** That's the class that's actually be used as timer for each 
     *  file object. */
    class TimerData
    {
    private:
        /** The filename. Note that this is usually */
        std::string m_name;
        /** The actual timer. */
        Timer       m_timers[TIMER_COUNT];
        /** How often a function was called. */
        unsigned long m_count[TIMER_COUNT];
        /** E.g. number of bytes read or written. */
        unsigned long m_amount[TIMER_COUNT];

    public:
        TimerData(const std::string &name)
        {
            m_name = name;
            for(unsigned int i=0; i<TIMER_COUNT; i++)
            {
                m_count[i]  = 0;
                m_amount[i] = 0;
            }
        }
        // --------------------------------------------------------------------
        void setName(const std::string &name) 
        { 
            m_name = name;
        }
        // --------------------------------------------------------------------
        const std::string& getName() const { return m_name; }
        // --------------------------------------------------------------------
        void start(int n) { m_count[n]++; m_timers[n].start(); }
        // --------------------------------------------------------------------
        void start(int n, unsigned long amount) 
        { 
            m_amount[n] += amount;
            m_count[n]++; 
            m_timers[n].start(); 
        }   // start
        // --------------------------------------------------------------------
        double stop(int n) { return m_timers[n].stop(); }
        // --------------------------------------------------------------------
        double stop(int n, unsigned long amount) 
        { 
            m_amount[n] += amount;
            return m_timers[n].stop(); 
        }
        // --------------------------------------------------------------------
        unsigned long getCount(int n) const { return m_count[n]; }
        // --------------------------------------------------------------------
        unsigned long getAmount(int n) const { return m_amount[n]; }
        // --------------------------------------------------------------------
        double getTime(int n) const { return m_timers[n].getSum(); }
    };   // TimerData

    typedef std::vector<TimerData*> AllTimerDataType;

    /** This static object must be a pointer to the class, not a class. If it
     *  would be the class, the destructor would be called before the destructor
     *  of the shared library, meaning we can't print the stats anymore.
     */
    static AllTimerDataType *m_all_timers;

    static TimerData *getTimer(const std::string &filename);

    
    TimerData *m_timers;

public:

    static void printTable(FILE *out);

    TimerFileObjectDecorator(I_FileObject *parent)
        : I_FileObjectDecorator(parent)
    {
        m_timers = NULL;
    };
    // ------------------------------------------------------------------------
    virtual ~TimerFileObjectDecorator() {};
    // ------------------------------------------------------------------------
    virtual void setFilename(const std::string &filename)
    {
        I_FileObjectDecorator::setFilename(filename);
        m_timers = getTimer(filename);
    }
    // ------------------------------------------------------------------------
    virtual FILE* fopen(const char *mode)
    {
        m_timers->start(TIMER_OPEN);
        FILE *file = I_FileObjectDecorator::fopen(mode);
        m_timers->stop(TIMER_OPEN);
        return (FILE*)this;
    }
    // ------------------------------------------------------------------------
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb)
    {
        m_timers->start(TIMER_WRITE);
        size_t n = I_FileObjectDecorator::fwrite(ptr, size, nmemb);
        m_timers->stop(TIMER_WRITE, n);
        return n;
    }
    // ------------------------------------------------------------------------
    virtual size_t fread(void *ptr,size_t size, size_t nmemb) 
    { 
        m_timers->start(TIMER_READ);
        size_t n = I_FileObjectDecorator::fread(ptr, size, nmemb);
        m_timers->stop(TIMER_READ, n);
        return n;
    }
    // ------------------------------------------------------------------------
    virtual int feof() 
    { 
        m_timers->start(TIMER_MISC);
        int n = I_FileObjectDecorator::feof();
        m_timers->stop(TIMER_MISC);
        return n;
    }   // feof
    // ------------------------------------------------------------------------
    virtual char * fgets(char *s, int size) 
    { 
        m_timers->start(TIMER_READ, size);
        char *result = I_FileObjectDecorator::fgets(s, size);
        m_timers->stop(TIMER_READ);
        return result;
    }   // fgets
    // ------------------------------------------------------------------------
    virtual int fclose() 
    {
        m_timers->start(TIMER_CLOSE);
        int error = I_FileObjectDecorator::fclose(); 
        m_timers->stop(TIMER_CLOSE);
        return error;
    }   // fclose

    // ------------------------------------------------------------------------
    virtual int open(int flags, mode_t mode)
    {
        m_timers->start(TIMER_OPEN);
        int filedes = I_FileObjectDecorator::open(flags, mode);
        m_timers->stop(TIMER_OPEN);
        return filedes;
    }   // open
    // ------------------------------------------------------------------------
    virtual int __xstat(int ver, struct stat *buf)
    {
        m_timers->start(TIMER_MISC);
        int error = I_FileObjectDecorator::__xstat(ver, buf);
        m_timers->stop(TIMER_MISC);
        return error;
    }   // fstat
    // ------------------------------------------------------------------------
    virtual int __fxstat(int ver, struct stat *buf)
    {
        m_timers->start(TIMER_MISC);
        int error = I_FileObjectDecorator::__fxstat(ver, buf);
        m_timers->stop(TIMER_MISC);
        return error;
    }   // fstat
    // ------------------------------------------------------------------------
    virtual int __lxstat(int ver, struct stat *buf)
    {
        m_timers->start(TIMER_MISC);
        int error = I_FileObjectDecorator::__lxstat(ver, buf);
        m_timers->stop(TIMER_MISC);
        return error;
    }   // fstat
    // ------------------------------------------------------------------------
    virtual off_t lseek(off_t offset, int whence)
    {
        m_timers->start(TIMER_SEEK);
        int error = I_FileObjectDecorator::lseek(offset, whence);
        m_timers->stop(TIMER_SEEK);
        return error;
    }   // lseek
    // ------------------------------------------------------------------------
    virtual ssize_t write(const void *buf, size_t nbyte)
    {
        m_timers->start(TIMER_WRITE);
        ssize_t result = I_FileObjectDecorator::write(buf, nbyte);
        m_timers->stop(TIMER_WRITE, result);
        return result;
    }   // write
    // ------------------------------------------------------------------------
    virtual ssize_t read(void *buf, size_t count)
    {
        m_timers->start(TIMER_READ);
        ssize_t result = I_FileObjectDecorator::read(buf, count);
        m_timers->stop(TIMER_READ, result);
        return result;
    }   // read
    // ------------------------------------------------------------------------
    virtual int close()
    {
        m_timers->start(TIMER_CLOSE);
        int result = I_FileObjectDecorator::close();
        m_timers->stop(TIMER_CLOSE);
        return result;
    }   // close
    // ------------------------------------------------------------------------
    virtual int rename(const char *newpath)
    {
        m_timers->start(TIMER_MISC);
        int result = I_FileObjectDecorator::rename(newpath);
        m_timers->stop(TIMER_MISC);
        return result;
    }
    // ------------------------------------------------------------------------

};   // TimeFileObjectDecorator

}   // namespace AIO
#endif
