#include "client/remote.hpp"

namespace AIO
{
Remote::Remote()
{
};   // Remote

// ------------------------------------------------------------------------
 Remote::~Remote()
{
};   // ~Remote

// ------------------------------------------------------------------------
 FILE* Remote:: fopen(const char *mode)
{
    return NULL;
}   // fopen

    // ------------------------------------------------------------------------
size_t Remote::fwrite(const void *ptr,size_t size, size_t nmemb)
{
    return nmemb;
}   // f_write

    // ------------------------------------------------------------------------
int Remote::fclose()
{
    return 0;
}   // fclose

    // ------------------------------------------------------------------------
off_t Remote::lseek(int fildes, off_t offset, int whence)
{
    return 0;
}   // lseek


}   // namespace AIO
