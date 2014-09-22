/*
 * PJSIP persistent document implementation based on pugixml backend
 * -----------------------------------------------------------------
 *
 * This library is distributed under the MIT License.
 *
 * Copyright (C) 2014, by Aleksei Kharlov (akharlov@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
#include <stdexcept>
#include <iostream>
#include "pjsettings-pugixml.h"

using namespace pj;
using namespace std;

namespace pjsettings
{

    /* Pugixml node operations */
    static bool          pugixmlNode_hasUnread(const ContainerNode*);
    static string        pugixmlNode_unreadName(const ContainerNode*n) throw(Error);
    static float         pugixmlNode_readNumber(const ContainerNode*, const string&) throw(Error);
    static bool          pugixmlNode_readBool(const ContainerNode*, const string&) throw(Error);
    static string        pugixmlNode_readString(const ContainerNode*, const string&) throw(Error);
    static StringVector  pugixmlNode_readStringVector(const ContainerNode*, const string&) throw(Error);
    static ContainerNode pugixmlNode_readContainer(const ContainerNode*, const string &) throw(Error);
    static ContainerNode pugixmlNode_readArray(const ContainerNode*, const string &) throw(Error);
    static void          pugixmlNode_writeNumber(ContainerNode*, const string &name, float num) throw(Error);
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

    PjPugixmlDocument::PjPugixmlDocument()
        : _document()
        , _rootNode()
    {
    }

    void PjPugixmlDocument::initRoot()
    {
        pugi::xml_node rootElement = _document.root().first_child();
        _rootNode.op = &pugixml_op;
        _rootNode.data.doc = this;
        _rootNode.data.data1 = rootElement.internal_object();
        _rootNode.data.data2 = NULL;
    }

    void PjPugixmlDocument::loadFile(const std::string &filename) throw(pj::Error)
    {
        pugi::xml_parse_result result = _document.load_file(filename.c_str());
        if (!result)
        {
            throw Error(1, "pugixml load from file error", result.description(), filename.c_str(), result.offset);
        }
        initRoot();
    }

    void PjPugixmlDocument::loadString(const std::string &input) throw(pj::Error)
    {
        pugi::xml_parse_result result = _document.load(input.c_str());
        if (!result)
        {
            throw Error(1, "pugixml load from string error", result.description(), "offset", result.offset);
        }
        initRoot();
    }

    void PjPugixmlDocument::saveFile(const std::string &filename) throw(pj::Error)
    {
        throw std::runtime_error("not implemented");
    }

    std::string PjPugixmlDocument::saveString() throw(pj::Error)
    {
        throw std::runtime_error("not implemented");
    }

    pj::ContainerNode &PjPugixmlDocument::getRootContainer() const
    {
        return _rootNode;
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

    static float         pugixmlNode_readNumber(const ContainerNode *node, const string &name) throw(Error)
    {
        pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        pugi::xml_node element(data);
        return element.attribute(name.c_str()).as_double(0.0);
    }

    static bool          pugixmlNode_readBool(const ContainerNode *node, const string &name) throw(Error)
    {
        pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        pugi::xml_node element(data);
        return element.attribute(name.c_str()).as_bool(false);
    }

    static string        pugixmlNode_readString(const ContainerNode *node, const string &name) throw(Error)
    {
        pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        pugi::xml_node element(data);
        return element.attribute(name.c_str()).as_string("");
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

            pugi::xml_node nextSibling = arrayIterator.next_sibling();
            const_cast<ContainerNode*>(node)->data.data2 = nextSibling.internal_object();
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

            pugi::xml_node nextSibling = arrayIterator.next_sibling();
            const_cast<ContainerNode*>(node)->data.data2 = nextSibling.internal_object();
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
        pugi::xml_node element(data);
        pugi::xml_node child = element.child(name.c_str());
        pugi::xml_node firstArrayChild = child.first_child();
        ContainerNode childNode = {};
        childNode.op = &pugixml_op;
        childNode.data.doc = node->data.doc;
        childNode.data.data1 = child.internal_object();
        childNode.data.data2 = firstArrayChild.internal_object();
        return childNode;
    }

    static void          pugixmlNode_writeNumber(ContainerNode *node, const string &name, float num) throw(Error)
    {

    }

    static void          pugixmlNode_writeBool(ContainerNode *node, const string &name, bool value) throw(Error)
    {

    }

    static void          pugixmlNode_writeString(ContainerNode *node, const string &name, const string &value) throw(Error)
    {

    }

    static void          pugixmlNode_writeStringVector(ContainerNode *node, const string &name, const StringVector &value) throw(Error)
    {

    }

    static ContainerNode pugixmlNode_writeNewContainer(ContainerNode *node, const string &name) throw(Error)
    {
        return *node;
    }

    static ContainerNode pugixmlNode_writeNewArray(ContainerNode *node, const string &name) throw(Error)
    {
        return *node;
    }

}

