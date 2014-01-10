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

#include "client/config.hpp"
#include "client/standard_file_object.hpp"

#include "tools/os.hpp"
#include "xml/xml_node.hpp"

#include <stdio.h>
#include <string>
#include <sys/resource.h>
#include <sys/time.h>

namespace ALIO
{

Config *Config::m_config = NULL;

// ----------------------------------------------------------------------------
/** Creates and initialises either a master or a client configuration object.
 */
void Config::create(bool is_client)
{
    assert(!m_config);
    m_config = new Config(is_client);
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
/** Creates the once instance of the config object (one for server, one for
 *  client). This is called from the constructor of the alio dll
 *  (see init.cpp).
 *  \param is_client True if this is the 
 */
Config::Config(bool is_client) : m_is_client(is_client)
{
    struct rlimit rlim;
    getrlimit(RLIMIT_NOFILE, &rlim);
    m_max_files = rlim.rlim_max;
    FileObjectInfo::init();
    const std::string name("alio.xml");
    const XMLNode *root = new ALIO::XMLNode(name);
    readConfig(root);
    FileObjectInfo::callAllStaticInitFunctions();
}   // Config

// ----------------------------------------------------------------------------
/** Destructor.
 */
Config::~Config()
{
    FileObjectInfo::atExit();
}   // ~Config

// ----------------------------------------------------------------------------
/** Reads the actual config xml file.
 *  \param root XMLNode object of the root of the xml file.
 */
void Config::readConfig(const XMLNode *root)
{
    if(!root || root->getName()!="alio")
    {
        fprintf(stderr, "Can't open file '%s' - no alio node '%s' %d.\n", 
                root->getFilename().c_str(), root->getName().c_str(),
                root->getNumNodes()
                );
        //   if(root) delete root;
        //exit(-1);
    }
    const XMLNode *config = root->getNode(m_is_client ? "client" : "master");

    // For each file pattern create the "file object" info object:
    for(unsigned int i=0; i<config->getNumNodes(); i++)
    {
        FileObjectInfo *foi = new FileObjectInfo(config->getNode(i));
        m_all_file_object_info.push_back(foi);
    }

}   // readConfig

// ----------------------------------------------------------------------------
ALIO::I_FileObject *Config::createFileObject(const char *name)
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
    printf("not handling file '%s'.\n", name);
    return NULL;

}   // createFileObject

// ----------------------------------------------------------------------------
/** Find the I_FileObject for a given FILE structure. While generally the FILE 
 *  structure returned by ALIO to the application is a pointer to the 
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
    if(filedes<m_max_files)
        return NULL;
    int indx = filedes - m_max_files;
    if(indx<m_file_objects.size())
        return m_file_objects[indx];
    return NULL;

}   // getFileObject(int)

// ----------------------------------------------------------------------------
}   // namespace ALIO
