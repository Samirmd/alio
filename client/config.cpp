
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
    readConfig(name);
}   // Config
// ----------------------------------------------------------------------------
void Config::readConfig(const std::string &name)
{
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
    while(!OS::feof(file))
    {
        if(!OS::fgets(buffer, size+1, file)) 
            break;

        std::string s(buffer);
        if(s.size()>0 && s[s.size()-1]=='\n')
            s.erase(s.end()-1);
        if(s.size()>0)
        {
            FileObjectInfo *foi = new FileObjectInfo(s);
            m_all_file_object_info.push_back(foi);
        }
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
AIO::I_FileObject *Config::createFileObject(const char *name)
{
    std::string s_name(name);
    for(unsigned int i=0; i<m_all_file_object_info.size(); i++)
    {
        const FileObjectInfo *foi = m_all_file_object_info[i];
        if(foi->isApplicable(s_name))
        {
            I_FileObject *fo = foi->createFileObject(name);
            fo->setIndex(m_file_objects.size());
            m_file_objects.push_back(fo);
            return fo;
        }
    }
    return NULL;

}   // createFileObject

// ----------------------------------------------------------------------------
/** Find the I_FileObject for a given FILE structure. While generally the FILE 
 *  structure returned by AIO to the application is a pointer to the 
 *  I_FileObject we have to handle the case that e.g. a previously opened FILE
 *  structure is passed in (so we can't simply cast all FILE structures to 
 *  I_FileObject).
 *  We could either search in m_file_objects to find if the address of the
 *  FILE structure is in there. But to avoid this overhead each I_FileObject
 *  has an integer field as its first component, which stores the index of the
 *  I_FileObject in m_file_objects. It is still possible that by accident a 
 *  FILE structure containts a valid index number, so we still have to test
 *  if the pointer at the specified index is indeed the right one.
 */
I_FileObject *Config::getFileObject(FILE *file)
{
    I_FileObject *fo = (I_FileObject*)(file);
    for(unsigned int i=0; i<m_file_objects.size(); i++)
    {
        if(m_file_objects[i]==fo)
            return fo;
    }

    return NULL;

    int index = fo->getIndex();
    if(index>=0 && index <m_file_objects.size() &&
       fo==m_file_objects[index])
        return m_file_objects[index];

    return NULL;
}   // getFileObject(FILE*)

// ----------------------------------------------------------------------------
I_FileObject *Config::getFileObject(int filedes)
{
    if(filedes<1024)
        return NULL;
    int indx = filedes -1024;
    if(indx<m_file_objects.size())
        return m_file_objects[indx];
    return NULL;

}   // getFileObject(int)

// ----------------------------------------------------------------------------
}   // namespace AIO
