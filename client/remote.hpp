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

#ifdef USE_MPI

#ifndef HEADER_REMOTE_HPP
#define HEADER_REMOTE_HPP

#include "client/base_file_object.hpp"
#include "tools/os.hpp"

#include "mpi.h"

#include <string>

namespace ALIO
{
class XMLNode;

class Remote : public BaseFileObject
{
    /** True if the connection to the server was established.
     */
    static bool     m_connected;

    /** The intercommunicator used to communicate with the server. 
     */
    static MPI_Comm m_intercomm;

public:
    static int      init();
    static int      atExit();
    static int      connectToServer();
                    Remote(const XMLNode *info);
    virtual        ~Remote();
    virtual FILE   *fopen(const char *mode);
    virtual FILE   *fopen64(const char *mode);
    virtual int     setvbuf(char *buf, int mode, size_t size);
    virtual int     fseek(long offset, int whence);
    virtual int     fseeko(off_t offset, int whence);
    virtual int     fseeko64(off64_t offset, int whence);
    virtual long    ftell();
    virtual off_t   ftello();
    virtual off64_t ftello64();
    virtual int     fflush();
    virtual int     ferror();
    virtual int     fileno();
    virtual size_t  fwrite(const void *ptr,size_t size, size_t nmemb);
    virtual size_t  fread(void *ptr,size_t size, size_t nmemb);
    virtual int     feof();
    virtual char   *fgets(char *s, int size);
    virtual int     fclose();

    virtual int     open(int flags, mode_t mode);
    virtual int     open64(int flags, mode_t mode);
    virtual int     __xstat(int ver, struct stat *buf);
    virtual int     __fxstat(int ver, struct stat *buf);
    virtual int     __fxstat64(int ver, struct stat64 *buf);
    virtual int     __lxstat(int ver, struct stat *buf);
    virtual off_t   lseek(off_t offset, int whence);
    virtual off64_t lseek64(off64_t offset, int whence);
    virtual ssize_t write(const void *buf, size_t nbyte);
    virtual ssize_t read(void *buf, size_t count);
    virtual int     close();
    virtual int     rename(const char *newpath);

};   // Remote

};   // namespace ALIO
#endif

// USE_MPI
#endif
