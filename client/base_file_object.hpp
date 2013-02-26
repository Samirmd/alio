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

    /** This stores the index of this object in config's m_file_object
     *  array. This allows us for a quick lookup of this item in config,
     *  and to distringuish it from non-AIO managed file objects.
     */
    int m_index;

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
    /** Sets the index of this object in config's m_file_object array. */
    void setIndex(int index) { m_index = index; }
    // ------------------------------------------------------------------------
    /** Returns the index of this object in config's m_file_object array. */
    int getIndex() const { return m_index; }
    // ------------------------------------------------------------------------
};   // BaseFileObject

}   // namespace AIO
#endif
