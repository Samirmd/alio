//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2009 Joerg Henrichs
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_XML_NODE_HPP
#define HEADER_XML_NODE_HPP

#ifdef _MSC_VER
  typedef __int32          int32_t;
  typedef unsigned __int32 uint32_t;
  typedef __int64          int64_t;
  typedef unsigned __int64 uint64_t;
#else
#  include <stdint.h>
#endif

#include <string>
#include <map>
#include <vector>

#include "irrString.h"
#include "irrXML.h"

using namespace irr;


class InterpolationArray;
class Vec3;

namespace AIO {
class XMLNode
{
private:
    /** Name of this element. */
    std::string                          m_name;
    /** List of all attributes. */
    std::map<std::string, core::stringc> m_attributes;
    /** List of all sub nodes. */
    std::vector<XMLNode *>               m_nodes;
    
    void readXML(io::IrrXMLReader *xml);
    
    std::string                          m_file_name;
    
public:
         XMLNode(io::IrrXMLReader *xml);
    
         /** \throw runtime_error if the file is not found */
         XMLNode(const std::string &filename);
    
        ~XMLNode();
        
    const std::string &getFilename() const { return m_file_name; }
    const std::string &getName() const {return m_name; }
    const XMLNode     *getNode(const std::string &name) const;
    void               getNodes(const std::string &s, std::vector<XMLNode*>& out) const;
    const XMLNode     *getNode(unsigned int i) const;
    unsigned int       getNumNodes() const {return m_nodes.size(); }
    int get(const std::string &attribute, std::string *value) const;
    int get(const std::string &attribute, int32_t  *value) const;
    int get(const std::string &attribute, uint32_t *value) const;
    int get(const std::string &attribute, int64_t  *value) const;
    int get(const std::string &attribute, float *value) const;
    int get(const std::string &attribute, bool *value) const;
    int get(const std::string &attribute, Vec3 *value) const;
    int get(const std::string &attribute, std::vector<std::string> *value) const;
    int get(const std::string &attribute, std::vector<float> *value) const;
    int get(const std::string &attribute, std::vector<int> *value) const;

};   // XMLNode

}   // namespace AIO
#endif
