#ifndef HEADER_CONFIG_HPP
#define HEADER_CONFIG_HPP

#include "client/file_object_info.hpp"
#include "client/i_file_object.hpp"

#include <assert.h>
#include <string>
#include <vector>

/** Stores the configuration of a client. It reads the configuration
 *  from a file/
 */
namespace AIO
{

class XMLNode;

class Config
{

private:
    /** Pointer to the one instance of this class. */
    static Config *m_config;

    /** All defined file information. */
    std::vector<FileObjectInfo*> m_all_file_object_info;

    /** Stores pointer to the original file objects. */
    std::vector<I_FileObject*> m_file_objects;

    Config();
   ~Config();
   void readConfig(XMLNode *root);
public:
   static void create();
   static void destroy();
   static Config *get()
   {
       return m_config;
   }   // get

    I_FileObject *createFileObject(const char *name);
    I_FileObject *getFileObject(FILE *file);
    I_FileObject *getFileObject(int filedes);
};   // Config

}   // namespace AIO
#endif
