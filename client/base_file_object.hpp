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

#ifndef HEADER_BASE_FILE_OBJECT_HPP
#define HEADER_BASE_FILE_OBJECT_HPP

#include "client/i_file_object.hpp"

namespace ALIO
{

class BaseFileObject : public I_FileObject
{
protected:
    /** Store the filename.*/
    std::string m_filename;

    /** This stores the index of this object in config's m_file_object
     *  array. This allows us for a quick lookup of this item in config,
     *  and to distringuish it from non-ALIO managed file objects.
     */
    int m_index;

public:
    BaseFileObject(const XMLNode *info) : I_FileObject(info)
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

}   // namespace ALIO
#endif
