/*
 * PJSIP persistent document implementation based on pugixml backend
 * -----------------------------------------------------------------
 *
 * Copyright (C) 2014, by Aleksei Kharlov (akharlov@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "pjsettings-pugixml.h"

using namespace pj;
using namespace std;

namespace pjsettings
{

    /* Pugixml node operations */
    static bool          pugixmlNode_hasUnread(const ContainerNode*);
    static string        pugixmlNode_unreadName(const ContainerNode*n) throw(Error);
    static double        pugixmlNode_readNumber(const ContainerNode*, const string&) throw(Error);
    static bool          pugixmlNode_readBool(const ContainerNode*, const string&) throw(Error);
    static string        pugixmlNode_readString(const ContainerNode*, const string&) throw(Error);
    static StringVector  pugixmlNode_readStringVector(const ContainerNode*, const string&) throw(Error);
    static ContainerNode pugixmlNode_readContainer(const ContainerNode*, const string &) throw(Error);
    static ContainerNode pugixmlNode_readArray(const ContainerNode*, const string &) throw(Error);
    static void          pugixmlNode_writeNumber(ContainerNode*, const string &name, double num) throw(Error);
    static void          pugixmlNode_writeBool(ContainerNode*, const string &name, bool value) throw(Error);
    static void          pugixmlNode_writeString(ContainerNode*, const string &name, const string &value) throw(Error);
    static void          pugixmlNode_writeStringVector(ContainerNode*, const string &name, const StringVector &value) throw(Error);
    static ContainerNode pugixmlNode_writeNewContainer(ContainerNode*, const string &name) throw(Error);
    static ContainerNode pugixmlNode_writeNewArray(ContainerNode*, const string &name) throw(Error);

    static container_node_op pugixml_op = {
        &pugixmlNode_hasUnread,
        &pugixmlNode_unreadName,
        &pugixmlNode_readNumber,
        &pugixmlNode_readBool,
        &pugixmlNode_readString,
        &pugixmlNode_readStringVector,
        &pugixmlNode_readContainer,
        &pugixmlNode_readArray,
        &pugixmlNode_writeNumber,
        &pugixmlNode_writeBool,
        &pugixmlNode_writeString,
        &pugixmlNode_writeStringVector,
        &pugixmlNode_writeNewContainer,
        &pugixmlNode_writeNewArray
    };

    PugixmlDocument::PugixmlDocument(unsigned int flags)
        : _document()
        , _rootNode()
        , _flags(flags)
    {
        _document.root().append_child("root");
        initRoot();
    }

    void PugixmlDocument::initRoot()
    {
        pugi::xml_node rootElement = _document.root().first_child();
        _rootNode.op = &pugixml_op;
        _rootNode.data.doc = this;
        _rootNode.data.data1 = rootElement.internal_object();
        _rootNode.data.data2 = NULL;
    }

    void PugixmlDocument::loadFile(const std::string &filename) throw(pj::Error)
    {
        pugi::xml_parse_result result = _document.load_file(filename.c_str());
        if (!result)
        {
            throw Error(1, "pugixml load from file error", result.description(), filename.c_str(), result.offset);
        }
        initRoot();
    }

    void PugixmlDocument::loadString(const std::string &input) throw(pj::Error)
    {
        pugi::xml_parse_result result = _document.load(input.c_str());
        if (!result)
        {
            throw Error(1, "pugixml load from string error", result.description(), "offset", result.offset);
        }
        initRoot();
    }

    void PugixmlDocument::saveFile(const std::string &filename) throw(pj::Error)
    {
        try
        {
            _document.save_file(
                filename.c_str(),
                "    ",
                _flags,
                pugi::encoding_utf8);
        }
        catch (std::exception &ex)
        {
            throw Error(1, "pugixml save to file error", ex.what(), "", 0);
        }
    }

    std::string PugixmlDocument::saveString() throw(pj::Error)
    {
        try
        {
            std::ostringstream result;
            _document.save(
                result,
                "    ",
                _flags,
                pugi::encoding_utf8);
            return result.str();
        }
        catch (std::exception &ex)
        {
            throw Error(1, "pugixml save to string error", ex.what(), "", 0);
        }
    }

    pj::ContainerNode &PugixmlDocument::getRootContainer() const
    {
        return _rootNode;
    }

    void selectNextArrayElement(const ContainerNode *node, const pugi::xml_node &arrayIterator)
    {
        pugi::xml_node nextSibling = arrayIterator.next_sibling();
        const_cast<ContainerNode*>(node)->data.data2 = nextSibling.internal_object();
    }

    static bool          pugixmlNode_hasUnread(const ContainerNode *node)
    {
        pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        if (arrayData != NULL)
        {
            pugi::xml_node arrayIterator(arrayData);
            return !!arrayIterator;
        }
        else
        {
            return false;
        }
    }

    static string        pugixmlNode_unreadName(const ContainerNode *node) throw(Error)
    {
        pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        if (arrayData != NULL)
        {
            pugi::xml_node arrayIterator(arrayData);
            return arrayIterator.name();
        }
        else
        {
           pugi::xml_node element(data);
            return element.name();
        }
    }

    static double         pugixmlNode_readNumber(const ContainerNode *node, const string &name) throw(Error)
    {
        pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        if (arrayData != NULL)
        {
            pugi::xml_node arrayIterator(arrayData);
            selectNextArrayElement(node, arrayIterator);
            return arrayIterator.text().as_double(0.0);
        }
        else
        {
            pugi::xml_node element(data);
            return element.attribute(name.c_str()).as_double(0.0);
        }
    }

    static bool          pugixmlNode_readBool(const ContainerNode *node, const string &name) throw(Error)
    {
        pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        if (arrayData != NULL)
        {
            pugi::xml_node arrayIterator(arrayData);
            selectNextArrayElement(node, arrayIterator);
            return arrayIterator.text().as_bool(false);
        }
        else
        {
            pugi::xml_node element(data);
            return element.attribute(name.c_str()).as_bool(false);
        }
    }

    static string        pugixmlNode_readString(const ContainerNode *node, const string &name) throw(Error)
    {
        pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        if (arrayData != NULL)
        {
            pugi::xml_node arrayIterator(arrayData);
            selectNextArrayElement(node, arrayIterator);
            return arrayIterator.text().as_string("");
        }
        else
        {
            pugi::xml_node element(data);
            return element.attribute(name.c_str()).as_string("");
        }
    }

    static StringVector  pugixmlNode_readStringVector(const ContainerNode *node, const string &name) throw(Error)
    {
        pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        pugi::xml_node stringVectorNode;
        if (arrayData != NULL)
        {
            pugi::xml_node arrayIterator(arrayData);
            stringVectorNode = arrayIterator;
            selectNextArrayElement(node, arrayIterator);
        }
        else
        {
            pugi::xml_node element(data);
            stringVectorNode = element.child(name.c_str());
        }

        StringVector result;
        for (pugi::xml_node item = stringVectorNode.first_child(); item; item = item.next_sibling())
        {
            const char *stringItem = item.text().as_string("");
            result.push_back(stringItem);
        }
        return result;
    }

    static ContainerNode pugixmlNode_readContainer(const ContainerNode *node, const string &name) throw(Error)
    {
        pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        if (arrayData != NULL)
        {
            pugi::xml_node arrayIterator(arrayData);
            ContainerNode childNode = {};
            childNode.op = &pugixml_op;
            childNode.data.doc = node->data.doc;
            childNode.data.data1 = arrayIterator.internal_object();

            selectNextArrayElement(node, arrayIterator);
            return childNode;
        }
        else
        {
            pugi::xml_node element(data);
            pugi::xml_node child = element.child(name.c_str());
            ContainerNode childNode = {};
            childNode.op = &pugixml_op;
            childNode.data.doc = node->data.doc;
            childNode.data.data1 = child.internal_object();
            return childNode;
        }
    }

    static ContainerNode pugixmlNode_readArray(const ContainerNode *node, const string &name) throw(Error)
    {
        pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        pugi::xml_node workNode;
        if (arrayData != NULL)
        {
            workNode = pugi::xml_node(arrayData);
            selectNextArrayElement(node, workNode);
        }
        else
        {
            pugi::xml_node element(data);
            workNode = element.child(name.c_str());
        }
        pugi::xml_node firstArrayChild = workNode.first_child();
        ContainerNode childNode = {};
        childNode.op = &pugixml_op;
        childNode.data.doc = node->data.doc;
        childNode.data.data1 = workNode.internal_object();
        childNode.data.data2 = firstArrayChild.internal_object();
        return childNode;
    }

    static void          pugixmlNode_writeNumber(ContainerNode *node, const string &name, double num) throw(Error)
    {
        pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        if (arrayData != NULL)
        {
            pugi::xml_node arrayIterator(arrayData);
            arrayIterator.append_child("item").text().set(num);
        }
        else
        {
            pugi::xml_node element(data);
            element.append_attribute(name.c_str()).set_value(num);
        }
    }

    static void          pugixmlNode_writeBool(ContainerNode *node, const string &name, bool value) throw(Error)
    {
        pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        if (arrayData != NULL)
        {
            pugi::xml_node arrayIterator(arrayData);
            arrayIterator.append_child("item").text().set(value);
        }
        else
        {
            pugi::xml_node element(data);
            element.append_attribute(name.c_str()).set_value(value);
        }
    }

    static void          pugixmlNode_writeString(ContainerNode *node, const string &name, const string &value) throw(Error)
    {
        pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        if (arrayData != NULL)
        {
            pugi::xml_node arrayIterator(arrayData);
            arrayIterator.append_child("item").text().set(value.c_str());
        }
        else
        {
            pugi::xml_node element(data);
            element.append_attribute(name.c_str()).set_value(value.c_str());
        }
    }

    static void          pugixmlNode_writeStringVector(ContainerNode *node, const string &name, const StringVector &value) throw(Error)
    {
        pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        pugi::xml_node workNode;
        if (arrayData != NULL)
        {
            pugi::xml_node arrayIterator(arrayData);
            workNode = arrayIterator.append_child(name.c_str());
        }
        else
        {
            pugi::xml_node element(data);
            workNode = element.append_child(name.c_str());
        }

        for (StringVector::const_iterator it = value.begin(); it != value.end(); ++it)
        {
            workNode.append_child("item").text().set((*it).c_str());
        }
    }

    static ContainerNode pugixmlNode_writeNewContainer(ContainerNode *node, const string &name) throw(Error)
    {
        pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        pugi::xml_node workNode;
        if (arrayData != NULL)
        {
            pugi::xml_node arrayIterator(data);
            workNode = arrayIterator.append_child(name.c_str());
        }
        else
        {
            pugi::xml_node element(data);
            workNode = element.append_child(name.c_str());
        }
        ContainerNode childNode = {};
        childNode.op = &pugixml_op;
        childNode.data.doc = node->data.doc;
        childNode.data.data1 = workNode.internal_object();
        return childNode;
    }

    static ContainerNode pugixmlNode_writeNewArray(ContainerNode *node, const string &name) throw(Error)
    {
        pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        pugi::xml_node workNode;
        if (arrayData != NULL)
        {
            pugi::xml_node arrayIterator(arrayData);
            workNode = arrayIterator.append_child(name.c_str());
        }
        else
        {
            pugi::xml_node element(data);
            workNode = element.append_child(name.c_str());
        }
        ContainerNode childNode = {};
        childNode.op = &pugixml_op;
        childNode.data.doc = node->data.doc;
        childNode.data.data1 = workNode.internal_object();
        childNode.data.data2 = workNode.internal_object();
        return childNode;
    }

}

