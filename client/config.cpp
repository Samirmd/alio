
#include "client/config.hpp"
#include "client/standard_file_object.hpp"

#include "tools/os.hpp"

#include <stdio.h>
#include <string>

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
        OS::fprintf(stderr, "Can't open file '%s' - aborting.\n", name.c_str());
        exit(-1);
    }
    // We want to avoid using fstreams here, since they call internally
    // open, read, write, close etc - which might get redirected. So
    // only use standard C calls.

    OS::fseek(file, 0L, SEEK_END);
    unsigned long size = ftell(file);
    OS::fseek(file, 0L, SEEK_SET);

    char *buffer = new char[size+1];
   
    if(!buffer)
    {
        fprintf(stderr, "Can't allocate %ld bytes for config file '%s' - aborting.\n",
                size, name.c_str());
        exit(-1);
    }

    while(!feof(file))
    {
        if(!fgets(buffer, size+1, file))
        {
            fprintf(stderr, "Problems reading file '%s' - aborting.\n",
                    name.c_str());
            exit(-1);
        }
        std::string s(buffer);
        if(s[s.size()-1]=='\n')
            s.erase(s.end()-1);
        printf("Read '%s'.\n", s.c_str());
    }
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
