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

    void selectNextArrayElement(const ContainerNode *node, ptrdiff_t currentIndex)
    {
        const_cast<ContainerNode*>(node)->data.data2 = reinterpret_cast<void*>(++currentIndex);
    }

    static Json::Value &get_value(const ContainerNode *node)
    {
        Json::Value *data = static_cast<Json::Value *>(node->data.data1);
        if (data == NULL)
        {
            throw pj::Error(1, "get_value error", "parent node data is null", "", 0);
        }
        return *data;
    }

    static ArrayIndex get_array_index(const ContainerNode *node)
    {
        return static_cast<ArrayIndex>(reinterpret_cast<size_t>(node->data.data2));
    }

    static Json::Value &get_array_value(Json::Value &data, ArrayIndex arrayIndex)
    {
        if (!data.isValidIndex(arrayIndex - 1))
        {
            throw pj::Error(1, "read container error", "no more container items in array", "", arrayIndex);
        }
        return data[arrayIndex - 1];
    }

    static bool          jsoncppNode_hasUnread(const ContainerNode *node)
    {
        Json::Value &data = get_value(node);
        ArrayIndex arrayIndex = get_array_index(node);
        if (arrayIndex > 0)
        {
            return data.isValidIndex(arrayIndex - 1);
        }
        else
        {
            return false;
        }
    }

    static string        jsoncppNode_unreadName(const ContainerNode *node) throw(Error)
    {
        // There is no name property for json values
        return "";
    }

    static float         jsoncppNode_readNumber(const ContainerNode *node, const string &name) throw(Error)
    {
        Json::Value &data = get_value(node);
        ArrayIndex arrayIndex = get_array_index(node);
        if (arrayIndex > 0)
        {
            Json::Value &arrayElement = get_array_value(data, arrayIndex);
            selectNextArrayElement(node, arrayIndex);
            return arrayElement.asDouble();
        }
        else
        {
            Json::Value &element = data[name];
            return element.asDouble();
        }
    }

    static bool          jsoncppNode_readBool(const ContainerNode *node, const string &name) throw(Error)
    {
        Json::Value &data = get_value(node);
        ArrayIndex arrayIndex = get_array_index(node);
        if (arrayIndex > 0)
        {
            Json::Value &arrayElement = get_array_value(data, arrayIndex);
            selectNextArrayElement(node, arrayIndex);
            return arrayElement.asBool();
        }
        else
        {
            Json::Value &element = data[name];
            return element.asBool();
        }
    }

    static string        jsoncppNode_readString(const ContainerNode *node, const string &name) throw(Error)
    {
        Json::Value &data = get_value(node);
        ArrayIndex arrayIndex = get_array_index(node);
        if (arrayIndex > 0)
        {
            Json::Value &arrayElement = get_array_value(data, arrayIndex);
            selectNextArrayElement(node, arrayIndex);
            return arrayElement.asString();
        }
        else
        {
            Json::Value &element = data[name];
            return element.asString();
        }
    }

    static StringVector  jsoncppNode_readStringVector(const ContainerNode *node, const string &name) throw(Error)
    {
        Json::Value &data = get_value(node);
        ArrayIndex arrayIndex = get_array_index(node);
        Json::Value *stringVectorNode = NULL;
        if (arrayIndex > 0)
        {
            stringVectorNode = &get_array_value(data, arrayIndex);
            selectNextArrayElement(node, arrayIndex);
        }
        else
        {
            stringVectorNode = &data[name];
        }

        Json::Value &element = *stringVectorNode;
        StringVector result;
        for (int i = 0; i < element.size(); ++i)
        {
            Json::Value &item = element[i];
            result.push_back(item.asCString());
        }
        return result;
    }

    static ContainerNode jsoncppNode_readContainer(const ContainerNode *node, const string &name) throw(Error)
    {
        Json::Value &data = get_value(node);
        ArrayIndex arrayIndex = get_array_index(node);
        if (arrayIndex > 0)
        {
            Json::Value &arrayElement = get_array_value(data, arrayIndex);

            ContainerNode childNode = {};
            childNode.op = &jsoncpp_op;
            childNode.data.doc = node->data.doc;
            childNode.data.data1 = &arrayElement;

            selectNextArrayElement(node, arrayIndex);
            return childNode;
        }
        else
        {
            Json::Value &element = data[name];
            ContainerNode childNode = {};
            childNode.op = &jsoncpp_op;
            childNode.data.doc = node->data.doc;
            childNode.data.data1 = &element;
            return childNode;
        }
    }

    static ContainerNode jsoncppNode_readArray(const ContainerNode *node, const string &name) throw(Error)
    {
        Json::Value &data = get_value(node);
        ArrayIndex arrayIndex = get_array_index(node);
        Json::Value *workData = NULL;
        if (arrayIndex > 0)
        {
            Json::Value &arrayElement = get_array_value(data, arrayIndex);
            workData = &arrayElement;
            selectNextArrayElement(node, arrayIndex);
        }
        else
        {
            Json::Value &element = data[name.c_str()];
            workData = &element;
        }

        if (!workData || !workData->isArray())
        {
            throw pj::Error(1, "read array error", "array expected", name, 0);
        }
        ContainerNode childNode = {};
        childNode.op = &jsoncpp_op;
        childNode.data.doc = node->data.doc;
        childNode.data.data1 = workData;
        childNode.data.data2 = reinterpret_cast<void*>(1);
        return childNode;
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

