
#include "tools/os.hpp"

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace AIO {
namespace OS  {
    t_open    open    = NULL;
    t_write   write   = NULL;
    t_close   close   = NULL;

    t_fopen   fopen   = NULL;
    t_fseek   fseek   = NULL;
    t_fwrite  fwrite  = NULL;
    t_fprintf fprintf = NULL;
    t_lseek   lseek   = NULL;
    t_fclose  fclose  = NULL;
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
    AIO::OS::open    = GET(t_open,    "open"   );
    AIO::OS::write   = GET(t_write,   "write"  );
    AIO::OS::lseek   = GET(t_lseek,   "lseek"  );
#endif

    AIO::OS::fopen   = GET(t_fopen,   "fopen"  );
    AIO::OS::fseek   = GET(t_fseek,   "fseek"  );
    AIO::OS::fwrite  = GET(t_fwrite,  "fwrite" );
    AIO::OS::fprintf = GET(t_fprintf, "fprintf");
    AIO::OS::fclose  = GET(t_fclose,  "fclose" );

    return 0;
}   // init

