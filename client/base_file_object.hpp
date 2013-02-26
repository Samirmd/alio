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

    /** The index in the global config array. This is used to */
    int m_index;
public:
    BaseFileObject() 
    {
    };
    // ------------------------------------------------------------------------
    virtual ~BaseFileObject() {};
    // ------------------------------------------------------------------------
    virtual void setData(const std::string &filename, int index)
    {
        m_filename = filename;
        m_index    = index;
    }   // setData

    // ------------------------------------------------------------------------
    virtual const std::string &getFilename() const { return m_filename; }
    // ------------------------------------------------------------------------
    virtual const int getIndex() const { return m_index; }
};   // BaseFileObject

}   // namespace AIO
#endif
