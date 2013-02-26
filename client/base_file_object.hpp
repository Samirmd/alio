#ifndef HEADER_BASE_FILE_OBJECT_HPP
#define HEADER_BASE_FILE_OBJECT_HPP

#include "client/i_file_object.hpp"

namespace AIO
{

class BaseFileObject : public I_FileObject
{
protected:
    /** Store the filename.*/
    std::string m_filename;

public:
    BaseFileObject() 
    {
    };
    // ------------------------------------------------------------------------
    virtual ~BaseFileObject() {};
    // ------------------------------------------------------------------------
    virtual void setFilename(const std::string &filename)
    {
        m_filename = filename;
    }   // setData

    // ------------------------------------------------------------------------
    virtual const std::string &getFilename() const { return m_filename; }
    // ------------------------------------------------------------------------
    virtual int getIndex() const { return m_index; }
};   // BaseFileObject

}   // namespace AIO
#endif
