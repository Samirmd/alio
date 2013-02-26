#include "client/config.hpp"
#include "client/standard_file_object.hpp"
#include "tools/os.hpp"

//#include <io.h>


FILE * xfopen(const char *FILE, const char *mode);
size_t xfwrite(const void *ptr,size_t size, size_t nmemb, FILE *stream);
int    xfclose(FILE *fp);


int main(int argc, char **argv)
{
    AIO::OS::init();
    AIO::Config::create();

    const char *message="Hello\n";

    FILE *f = xfopen("a", "w");
    xfwrite(message, 1, strlen(message), f);
    xfclose(f);
    return 0;
}   // main
