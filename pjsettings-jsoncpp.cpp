/*
 * PJSIP persistent document implementation based on jsoncpp backend
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
#include <stdexcept>
#include <iostream>
#include "pjsettings-jsoncpp.h"

using namespace pj;
using namespace Json;
using namespace std;

namespace pjsettings
{

    /* Pugixml node operations */
    static bool          jsoncppNode_hasUnread(const ContainerNode*);
    static string        jsoncppNode_unreadName(const ContainerNode*n) throw(Error);
    static float         jsoncppNode_readNumber(const ContainerNode*, const string&) throw(Error);
    static bool          jsoncppNode_readBool(const ContainerNode*, const string&) throw(Error);
    static string        jsoncppNode_readString(const ContainerNode*, const string&) throw(Error);
    static StringVector  jsoncppNode_readStringVector(const ContainerNode*, const string&) throw(Error);
    static ContainerNode jsoncppNode_readContainer(const ContainerNode*, const string &) throw(Error);
    static ContainerNode jsoncppNode_readArray(const ContainerNode*, const string &) throw(Error);
    static void          jsoncppNode_writeNumber(ContainerNode*, const string &name, float num) throw(Error);
    static void          jsoncppNode_writeBool(ContainerNode*, const string &name, bool value) throw(Error);
    static void          jsoncppNode_writeString(ContainerNode*, const string &name, const string &value) throw(Error);
    static void          jsoncppNode_writeStringVector(ContainerNode*, const string &name, const StringVector &value) throw(Error);
    static ContainerNode jsoncppNode_writeNewContainer(ContainerNode*, const string &name) throw(Error);
    static ContainerNode jsoncppNode_writeNewArray(ContainerNode*, const string &name) throw(Error);

    static container_node_op jsoncpp_op = {
        &jsoncppNode_hasUnread,
        &jsoncppNode_unreadName,
        &jsoncppNode_readNumber,
        &jsoncppNode_readBool,
        &jsoncppNode_readString,
        &jsoncppNode_readStringVector,
        &jsoncppNode_readContainer,
        &jsoncppNode_readArray,
        &jsoncppNode_writeNumber,
        &jsoncppNode_writeBool,
        &jsoncppNode_writeString,
        &jsoncppNode_writeStringVector,
        &jsoncppNode_writeNewContainer,
        &jsoncppNode_writeNewArray
    };

    PjJsonCppDocument::PjJsonCppDocument()
        : _document()
        , _rootNode()
    {
    }

    void PjJsonCppDocument::initRoot()
    {
        Value &rootElement = _document;
        _rootNode.op = &jsoncpp_op;
        _rootNode.data.doc = this;
        _rootNode.data.data1 = &_document;
        _rootNode.data.data2 = NULL;
    }

    void PjJsonCppDocument::loadFile(const std::string &filename) throw(pj::Error)
    {
        bool result = false;
        if (!result)
        {
            throw Error(1, "jsoncpp load from file error", "?", filename.c_str(), 0);
        }
        initRoot();
    }

    void PjJsonCppDocument::loadString(const std::string &input) throw(pj::Error)
    {
        Json::Reader reader;
        bool parsedSuccessfully = reader.parse(input, _document);
        if (!parsedSuccessfully)
        {
            throw Error(1, "jsoncpp load from string error", reader.getFormattedErrorMessages(), "offset", 0);
        }
        initRoot();
    }

    void PjJsonCppDocument::saveFile(const std::string &filename) throw(pj::Error)
    {
        throw std::runtime_error("not implemented");
    }

    std::string PjJsonCppDocument::saveString() throw(pj::Error)
    {
        throw std::runtime_error("not implemented");
    }

    pj::ContainerNode &PjJsonCppDocument::getRootContainer() const
    {
        return _rootNode;
    }

    void selectNextArrayElement(const ContainerNode *node, const Value &arrayIterator)
    {
        // Value &nextSibling = arrayIterator.;
        // const_cast<ContainerNode*>(node)->data.data2 = nextSibling.internal_object();
    }

    static bool          jsoncppNode_hasUnread(const ContainerNode *node)
    {
        // pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        // pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        // if (arrayData != NULL)
        // {
        //     pugi::xml_node arrayIterator(arrayData);
        //     return !!arrayIterator;
        // }
        // else
        // {
        //     return false;
        // }
    }

    static string        jsoncppNode_unreadName(const ContainerNode *node) throw(Error)
    {
        // pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        // pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        // if (arrayData != NULL)
        // {
        //     pugi::xml_node arrayIterator(arrayData);
        //     return arrayIterator.name();
        // }
        // else
        // {
        //     pugi::xml_node element(data);
        //     return element.name();
        // }
    }

    static float         jsoncppNode_readNumber(const ContainerNode *node, const string &name) throw(Error)
    {
        // pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        // pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        // if (arrayData != NULL)
        // {
        //     pugi::xml_node arrayIterator(arrayData);
        //     selectNextArrayElement(node, arrayIterator);
        //     return arrayIterator.text().as_double(0.0);
        // }
        // else
        // {
        //     pugi::xml_node element(data);
        //     return element.attribute(name.c_str()).as_double(0.0);
        // }
    }

    static bool          jsoncppNode_readBool(const ContainerNode *node, const string &name) throw(Error)
    {
        // pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        // pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        // if (arrayData != NULL)
        // {
        //     pugi::xml_node arrayIterator(arrayData);
        //     selectNextArrayElement(node, arrayIterator);
        //     return arrayIterator.text().as_bool(false);
        // }
        // else
        // {
        //     pugi::xml_node element(data);
        //     return element.attribute(name.c_str()).as_bool(false);
        // }
    }

    static string        jsoncppNode_readString(const ContainerNode *node, const string &name) throw(Error)
    {
        // pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        // pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        // if (arrayData != NULL)
        // {
        //     pugi::xml_node arrayIterator(arrayData);
        //     selectNextArrayElement(node, arrayIterator);
        //     return arrayIterator.text().as_string("");
        // }
        // else
        // {
        //     pugi::xml_node element(data);
        //     return element.attribute(name.c_str()).as_string("");
        // }
    }

    static StringVector  jsoncppNode_readStringVector(const ContainerNode *node, const string &name) throw(Error)
    {
        // pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        // pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        // pugi::xml_node stringVectorNode;
        // if (arrayData != NULL)
        // {
        //     pugi::xml_node arrayIterator(arrayData);
        //     stringVectorNode = arrayIterator;
        //     selectNextArrayElement(node, arrayIterator);
        // }
        // else
        // {
        //     pugi::xml_node element(data);
        //     stringVectorNode = element.child(name.c_str());
        // }

        // StringVector result;
        // for (pugi::xml_node item = stringVectorNode.first_child(); item; item = item.next_sibling())
        // {
        //     const char *stringItem = item.text().as_string("");
        //     result.push_back(stringItem);
        // }
        // return result;
    }

    static ContainerNode jsoncppNode_readContainer(const ContainerNode *node, const string &name) throw(Error)
    {
        // pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        // pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        // if (arrayData != NULL)
        // {
        //     pugi::xml_node arrayIterator(arrayData);
        //     ContainerNode childNode = {};
        //     childNode.op = &jsoncpp_op;
        //     childNode.data.doc = node->data.doc;
        //     childNode.data.data1 = arrayIterator.internal_object();

        //     selectNextArrayElement(node, arrayIterator);
        //     return childNode;
        // }
        // else
        // {
        //     pugi::xml_node element(data);
        //     pugi::xml_node child = element.child(name.c_str());
        //     ContainerNode childNode = {};
        //     childNode.op = &jsoncpp_op;
        //     childNode.data.doc = node->data.doc;
        //     childNode.data.data1 = child.internal_object();
        //     return childNode;
        // }
    }

    static ContainerNode jsoncppNode_readArray(const ContainerNode *node, const string &name) throw(Error)
    {
        // pugi::xml_node_struct *data = static_cast<pugi::xml_node_struct *>(node->data.data1);
        // pugi::xml_node_struct *arrayData = static_cast<pugi::xml_node_struct *>(node->data.data2);
        // pugi::xml_node workNode;
        // if (arrayData != NULL)
        // {
        //     workNode = pugi::xml_node(arrayData);
        //     selectNextArrayElement(node, workNode);
        // }
        // else
        // {
        //     pugi::xml_node element(data);
        //     workNode = element.child(name.c_str());
        // }
        // pugi::xml_node firstArrayChild = workNode.first_child();
        // ContainerNode childNode = {};
        // childNode.op = &jsoncpp_op;
        // childNode.data.doc = node->data.doc;
        // childNode.data.data1 = workNode.internal_object();
        // childNode.data.data2 = firstArrayChild.internal_object();
        // return childNode;
    }

    static void          jsoncppNode_writeNumber(ContainerNode *node, const string &name, float num) throw(Error)
    {

    }

    static void          jsoncppNode_writeBool(ContainerNode *node, const string &name, bool value) throw(Error)
    {

    }

    static void          jsoncppNode_writeString(ContainerNode *node, const string &name, const string &value) throw(Error)
    {

    }

    static void          jsoncppNode_writeStringVector(ContainerNode *node, const string &name, const StringVector &value) throw(Error)
    {

    }

    static ContainerNode jsoncppNode_writeNewContainer(ContainerNode *node, const string &name) throw(Error)
    {
        return *node;
    }

    static ContainerNode jsoncppNode_writeNewArray(ContainerNode *node, const string &name) throw(Error)
    {
        return *node;
    }

}

