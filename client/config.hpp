#ifndef HEADER_CONFIG_HPP
#define HEADER_CONFIG_HPP

#include "client/i_file_object.hpp"

#include <assert.h>
#include <string>

/** Stores the configuration of a client. It reads the configuration
 *  from a file/
 */
namespace AIO
{
class Config
{
private:
    /** Pointer to the one instance of this class. */
    static Config *m_config;

    Config();
   ~Config();
public:
   static void create();
   static void destroy();
   static Config *get()
   {
       assert(m_config);
       return m_config;
   }   // get

    I_FileObject *getFileObjectFor(const char *name);
};   // Config

}   // namespace AIO
#endif
