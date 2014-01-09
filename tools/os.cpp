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

#include "tools/os.hpp"

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace ALIO {
namespace OS  {
    t_open       open       = NULL;
    t_open       open64     = NULL;
    t___xstat    __xstat    = NULL;
    t___xstat64  __xstat64  = NULL;
    t___fxstat   __fxstat   = NULL;
    t___fxstat64 __fxstat64 = NULL;
    t___lxstat   __lxstat   = NULL;
    t___lxstat64 __lxstat64 = NULL;
    t_lseek    lseek        = NULL;
    t_lseek64  lseek64      = NULL;
    t_write    write        = NULL;
    t_read     read         = NULL;
    t_close    close        = NULL;

    t_fopen    fopen    = NULL;
    t_fopen    fopen64  = NULL;
    t_setvbuf  setvbuf  = NULL;
    t_fseek    fseek    = NULL;
    t_fseeko   fseeko   = NULL;
    t_fseeko64 fseeko64 = NULL;
    t_ftell    ftell    = NULL;
    t_ftello   ftello   = NULL;
    t_ftello   ftello64 = NULL;
    t_fflush   fflush   = NULL;
    t_ferror   ferror   = NULL;
    t_fileno   fileno   = NULL;
    t_fwrite   fwrite   = NULL;
    t_fread    fread    = NULL;
    t_fgets    fgets    = NULL;
    t_feof     feof     = NULL;
    t_fclose   fclose   = NULL;

    t_rename  rename    = NULL;
} }  // namespace ALIO::OS

/** This function saves the function pointers to the original IO functions.
 */
int ALIO::OS::init()
{
#ifdef WIN32
#  define GET(type,name) \
    (ALIO::OS::type)GetProcAddress(GetModuleHandle(TEXT("MSVCR90D.DLL")), name);
    ALIO::OS::open    = GET(t_open,    "_open"  );
    ALIO::OS::write   = GET(t_write,   "_write" );
    ALIO::OS::lseek   = GET(t_lseek,   "_lseek" );
#else
#  define GET(type,name) \
    (ALIO::OS::type)dlsym(RTLD_NEXT, name)
    ALIO::OS::open       = GET(t_open,       "open"      );
    ALIO::OS::open64     = GET(t_open,       "open64"    );
    ALIO::OS::__xstat    = GET(t___xstat,    "__xstat"   );
    ALIO::OS::__xstat64  = GET(t___xstat64,  "__xstat64" );
    ALIO::OS::__fxstat   = GET(t___fxstat,   "__fxstat"  );
    ALIO::OS::__fxstat64 = GET(t___fxstat64, "__fxstat64");
    ALIO::OS::__lxstat   = GET(t___lxstat,   "__lxstat"  );
    ALIO::OS::__lxstat64 = GET(t___lxstat64, "__lxstat64");
    ALIO::OS::lseek      = GET(t_lseek,      "lseek"     );
    ALIO::OS::lseek64    = GET(t_lseek64,    "lseek64"   );
    ALIO::OS::write      = GET(t_write,      "write"     );
    ALIO::OS::read       = GET(t_read,       "read"      );
    ALIO::OS::close      = GET(t_close,      "close"     );
#endif
    ALIO::OS::fopen    = GET(t_fopen,    "fopen"   );
    ALIO::OS::fopen64  = GET(t_fopen,    "fopen64" );
    ALIO::OS::setvbuf  = GET(t_setvbuf,  "setvbuf" );
    ALIO::OS::fseek    = GET(t_fseek,    "fseek"   );
    ALIO::OS::fseeko   = GET(t_fseeko,   "fseeko"  );
    ALIO::OS::fseeko64 = GET(t_fseeko64, "fseeko64");
    ALIO::OS::ftell    = GET(t_ftell,    "ftell"   );
    ALIO::OS::ftello   = GET(t_ftello,   "ftello"  );
    ALIO::OS::ftello64 = GET(t_ftello64, "ftello64");
    ALIO::OS::fflush   = GET(t_fflush,   "fflush"  );
    ALIO::OS::ferror   = GET(t_ferror,   "ferror"  );
    ALIO::OS::fileno   = GET(t_fileno,   "fileno"  );
    ALIO::OS::fwrite   = GET(t_fwrite,   "fwrite"  );
    ALIO::OS::fread    = GET(t_fread,    "fread"   );
    ALIO::OS::fgets    = GET(t_fgets,    "fgets"   );
    ALIO::OS::feof     = GET(t_feof,     "feof"    );
    ALIO::OS::fclose   = GET(t_fclose,   "fclose"  );
    ALIO::OS::rename   = GET(t_rename,   "rename"  );
    return 0;
}   // init

