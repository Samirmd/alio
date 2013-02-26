
#include "tools/os.hpp"

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace AIO {
namespace OS  {
    t_open     open     = NULL;
    t___xstat  __xstat  = NULL;
    t___fxstat __fxstat = NULL;
    t___lxstat __lxstat = NULL;
    t_lseek    lseek    = NULL;
    t_write    write    = NULL;
    t_read     read     = NULL;
    t_close    close    = NULL;

    t_fopen    fopen    = NULL;
    t_fseek    fseek    = NULL;
    t_fwrite   fwrite   = NULL;
    t_fread    fread    = NULL;
    t_feof     feof     = NULL;
    t_fgets    fgets    = NULL;
    t_fprintf  fprintf  = NULL;
    t_fclose   fclose   = NULL;

    t_rename  rename    = NULL;
} }  // namespace AIO::OS


int AIO::OS::init()
{
#ifdef WIN32
#  define GET(type,name) \
    (AIO::OS::type)GetProcAddress(GetModuleHandle(TEXT("MSVCR90D.DLL")), name);
    AIO::OS::open    = GET(t_open,    "_open"  );
    AIO::OS::write   = GET(t_write,   "_write" );
    AIO::OS::lseek   = GET(t_lseek,   "_lseek" );
#else
#  define GET(type,name) \
    (AIO::OS::type)dlsym(RTLD_NEXT, name)
    AIO::OS::open     = GET(t_open,    "open"   );
    AIO::OS::__xstat  = GET(t___xstat, "__xstat"  );
    AIO::OS::__fxstat = GET(t___fxstat, "__fxstat"  );
    AIO::OS::__lxstat = GET(t___lxstat, "__lxstat"  );
    AIO::OS::lseek    = GET(t_lseek,   "lseek"  );
    AIO::OS::write    = GET(t_write,   "write"  );
    AIO::OS::read     = GET(t_read,    "read"   );
    AIO::OS::close    = GET(t_close,   "close"  );
#endif
    AIO::OS::fopen    = GET(t_fopen,   "fopen"  );
    AIO::OS::fseek    = GET(t_fseek,   "fseek"  );
    AIO::OS::fwrite   = GET(t_fwrite,  "fwrite" );
    AIO::OS::fread    = GET(t_fread,   "fread"  );
    AIO::OS::feof     = GET(t_feof,    "feof"   );
    AIO::OS::fgets    = GET(t_fgets,   "fgets"  );
    AIO::OS::fprintf  = GET(t_fprintf, "fprintf");
    AIO::OS::fclose   = GET(t_fclose,  "fclose" );
    AIO::OS::rename   = GET(t_rename,  "rename" );
    return 0;
}   // init

