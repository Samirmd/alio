#include "client/config.hpp"
#include "client/standard_file_object.hpp"
#include "tools/os.hpp"

extern "C" void __attribute__((constructor)) my_init(void)
{
    AIO::OS::init();
    AIO::Config::create();
}
