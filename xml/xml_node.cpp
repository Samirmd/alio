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

#include "xml/xml_node.hpp"
#include "tools/string_utils.hpp"

#include <stdexcept>

namespace ALIO
{

XMLNode::XMLNode(io::IrrXMLReader *xml)
{
    m_file_name = "[unknown]";
    
    while(xml->getNodeType()!=io::EXN_ELEMENT && xml->read());
    readXML(xml);
}   // XMLNode

// ----------------------------------------------------------------------------
/** Reads a XML file and convert it into a XMLNode tree.
 *  \param filename Name of the XML file to read.
 */
XMLNode::XMLNode(const std::string &filename)
{
    m_file_name = filename;
    
    io::IrrXMLReader *xml = irr::io::createIrrXMLReader(filename.c_str());
    
    if (xml == NULL)
    {
        throw std::runtime_error("Cannot find file "+filename);
    }
    
    bool is_first_element = true;
    while(xml->read())
    {
        switch (xml->getNodeType()) 
        {
        case io::EXN_ELEMENT:
            {
                if(!is_first_element)
                {
                    fprintf(stderr, 
                            "More than one root element in '%s' - ignored.\n",
                            filename.c_str());
                }
                readXML(xml);
                is_first_element = false;
                break;
            }
        case io::EXN_ELEMENT_END:  break;   // Ignore all other types
        case io::EXN_UNKNOWN:      break;
        case io::EXN_COMMENT:      break;
        case io::EXN_TEXT:         break;
        default:                   break;
        }   // switch
    }   // while
    delete xml;
}   // XMLNode

// ----------------------------------------------------------------------------
/** Destructor. */
XMLNode::~XMLNode()
{
    for(unsigned int i=0; i<m_nodes.size(); i++)
    {
        delete m_nodes[i];
    }
    m_nodes.clear();
}   // ~XMLNode

// ----------------------------------------------------------------------------
/** Stores all attributes, and reads in all children.
 *  \param xml The XML reader.
 */
void XMLNode::readXML(io::IrrXMLReader *xml)
{
    m_name = std::string(core::stringc(xml->getNodeName()).c_str());

    for(unsigned int i=0; i<xml->getAttributeCount(); i++)
    {
        std::string   name  = core::stringc(xml->getAttributeName(i)).c_str();
        core::stringc value = xml->getAttributeValue(i);
        m_attributes[name] = value;
    }   // for i

    // If no children, we are done
    if(xml->isEmptyElement()) 
        return;

    /** Read all children elements. */
    while(xml->read())
    {
        switch (xml->getNodeType()) 
        {
        case io::EXN_ELEMENT:
            {
                XMLNode* n = new XMLNode(xml);
                n->m_file_name = m_file_name;
                m_nodes.push_back(n);
                break;
            }
        case io::EXN_ELEMENT_END:
            // End of this element found.
            return;
            break;
        case io::EXN_UNKNOWN:            break;
        case io::EXN_COMMENT:            break;
        case io::EXN_TEXT:               break;
        default:                         break;
        }   // switch
    }   // while
}   // readXML

// ----------------------------------------------------------------------------
/** Returns the i.th node.
 *  \param i Number of node to return.
 */
const XMLNode *XMLNode::getNode(unsigned int i) const
{
    return m_nodes[i];
}   // getNode

// ----------------------------------------------------------------------------
/** Returns the node with the given name.
 *  \param s Name of the node to return.
 */
const XMLNode *XMLNode::getNode(const std::string &s) const
{
    for(unsigned int i=0; i<m_nodes.size(); i++)
    {
        if(m_nodes[i]->getName()==s) return m_nodes[i];
    }
    return NULL;
}   // getNode

// ----------------------------------------------------------------------------
/** Returns all nodes with the given name.
 *  \param s Name of the nodes to return.
 *  \param s Vector that will be filled with output values.
 */
void XMLNode::getNodes(const std::string &s, std::vector<XMLNode*>& out) const
{
    for(unsigned int i=0; i<m_nodes.size(); i++)
    {
        if(m_nodes[i]->getName()==s)
        {
            out.push_back(m_nodes[i]);
        }
    }
}   // getNode

// ----------------------------------------------------------------------------
/** If 'attribute' was defined, set 'value' to the value of the
*   attribute and return 1, otherwise return 0 and do not change value.
*  \param attribute Name of the attribute.
*  \param value Value of the attribute.
*/
int XMLNode::get(const std::string &attribute, std::string *value) const
{
    if(m_attributes.size()==0) return 0;
    std::map<std::string, core::stringc>::const_iterator o;
    o = m_attributes.find(attribute);
    if(o==m_attributes.end()) return 0;
    *value=core::stringc(o->second).c_str();
    return 1;
}   // get

// ----------------------------------------------------------------------------
int XMLNode::get(const std::string &attribute, int *value) const
{
    std::string s;
    if(!get(attribute, &s)) return 0;

    if (!StringUtils::parseString<int>(s, value))
    {
        fprintf(stderr, "[XMLNode] WARNING: Expected int but found '%s' for attribute '%s' of node '%s' in file %s\n",
                s.c_str(), attribute.c_str(), m_name.c_str(), m_file_name.c_str());
        return 0;
    }
    
    return 1;
}   // get(int_t)

// ----------------------------------------------------------------------------
int XMLNode::get(const std::string &attribute, long *value) const
{
    std::string s;
    if(!get(attribute, &s)) return 0;

    if (!StringUtils::parseString<long>(s, value))
    {
        fprintf(stderr, "[XMLNode] WARNING: Expected long but found '%s' for attribute '%s' of node '%s' in file %s\n",
                s.c_str(), attribute.c_str(), m_name.c_str(), m_file_name.c_str());
        return 0;
    }
    
    return 1;
}   // get(long)


// ----------------------------------------------------------------------------
int XMLNode::get(const std::string &attribute, uint32_t *value) const
{
    std::string s;
    if(!get(attribute, &s)) return 0;
    
    if (!StringUtils::parseString<unsigned int>(s, value))
    {
        fprintf(stderr, "[XMLNode] WARNING: Expected uint but found '%s' for attribute '%s' of node '%s' in file %s\n",
                s.c_str(), attribute.c_str(), m_name.c_str(), m_file_name.c_str());
        return 0;
    }
    
    return 1;
}   // get(uint32_t)

// ----------------------------------------------------------------------------
int XMLNode::get(const std::string &attribute, float *value) const
{
    std::string s;
    if(!get(attribute, &s)) return 0;
    
    if (!StringUtils::parseString<float>(s, value))
    {
        fprintf(stderr, "[XMLNode] WARNING: Expected float but found '%s' for attribute '%s' of node '%s' in file %s\n",
                s.c_str(), attribute.c_str(), m_name.c_str(), m_file_name.c_str());
        return 0;
    }
        
    return 1;
}   // get(int)

// ----------------------------------------------------------------------------
int XMLNode::get(const std::string &attribute, bool *value) const
{
    std::string s;
    
    // FIXME: for some reason, missing attributes don't trigger that if???
    if(!get(attribute, &s)) return 0;
    *value = s[0]=='T' || s[0]=='t' || s[0]=='Y' || s[0]=='y' ||
             s=="#t"   || s   =="#T" || s=="1";
    return 1;
}   // get(bool)

// ----------------------------------------------------------------------------
/** If 'attribute' was defined, split the value of the attribute by spaces,
 *  set value to this vector array and return the number of elements. Otherwise
 *  return 0 and do not change value.
 *  \param attribute Name of the attribute.
 *  \param value Value of the attribute.
 */
int XMLNode::get(const std::string &attribute, 
                 std::vector<std::string> *value) const
{
    std::string s;
    if(!get(attribute, &s)) return 0;

    *value = StringUtils::split(s,' ');
    return value->size();
}   // get(vector<string>)

// ----------------------------------------------------------------------------
/** If 'attribute' was defined, split the value of the attribute by spaces,
 *  set value to this vector array and return the number of elements. Otherwise
 *  return 0 and do not change value.
 *  \param attribute Name of the attribute.
 *  \param value Value of the attribute.
 */
int XMLNode::get(const std::string &attribute, 
                 std::vector<float> *value) const
{
    std::string s;
    if(!get(attribute, &s)) return 0;

    std::vector<std::string> v = StringUtils::split(s,' ');
    value->clear();
    
    const unsigned int count = v.size();
    for (unsigned int i=0; i<count; i++)
    {
        float curr;
        if (!StringUtils::parseString<float>(v[i], &curr))
        {
            fprintf(stderr, "[XMLNode] WARNING: Expected float but found '%s' for attribute '%s' of node '%s' in file %s\n",
                    v[i].c_str(), attribute.c_str(), m_name.c_str(), m_file_name.c_str());
            return 0;
        }
        
        value->push_back(curr);
    }
    return value->size();
}   // get(vector<float>)

// ----------------------------------------------------------------------------
/** If 'attribute' was defined, split the value of the attribute by spaces,
 *  set value to this vector array and return the number of elements. Otherwise
 *  return 0 and do not change value.
 *  \param attribute Name of the attribute.
 *  \param value Value of the attribute.
 */
int XMLNode::get(const std::string &attribute, std::vector<int> *value) const
{
    std::string s;
    if(!get(attribute, &s)) return 0;

    std::vector<std::string> v = StringUtils::split(s,' ');
    value->clear();
    
    const unsigned int count = v.size();
    for (unsigned int i=0; i<count; i++)
    {
        int val;
        if (!StringUtils::parseString<int>(v[i], &val))
        {
            fprintf(stderr, "[XMLNode] WARNING: Expected int but found '%s' for attribute '%s' of node '%s'\n",
                    v[i].c_str(), attribute.c_str(), m_name.c_str());
            return 0;
        }
        
        value->push_back(val);
    }
    return value->size();
}   // get(vector<int>)

}   // namespace ALIO
