
#include "client/config.hpp"
#include "client/standard_file_object.hpp"

#include "tools/os.hpp"

#include <stdio.h>

namespace AIO
{

Config *Config::m_config = NULL;

// ----------------------------------------------------------------------------
void Config::create()
{
    assert(!m_config);
    m_config = new Config();
}   // create;

// ----------------------------------------------------------------------------
/** Destroys the one instance of Config.
 */
void Config::destroy()
{
    assert(m_config);
    delete m_config;
    m_config = NULL;
}   // destroy
// ----------------------------------------------------------------------------

Config::Config()
{
    const std::string name("aio.cfg");
    FILE *file = OS::fopen(name.c_str(), "r");
    if(!file)
    {
        OS::fprintf(stderr, "Can't open file '%s' - aborting.\n");
        exit(-1);
    }

    char *buffer=NULL;
    while(!feof(file))
    {
        unsigned long len;
        getline(&buffer, &len, file);
        std::string line(buffer);
        printf("Read: %s.\n", line.c_str());

    }
    free(buffer);
    fclose(file);

}   // Config

// ----------------------------------------------------------------------------
/** Destructor.
 */
Config::~Config()
{
}   // ~Config
// ----------------------------------------------------------------------------
I_FileObject *Config::getFileObjectFor(const char *name)
{
    return new StandardFileObject(name);
}   // check

// ----------------------------------------------------------------------------

}   // namespace AIO
