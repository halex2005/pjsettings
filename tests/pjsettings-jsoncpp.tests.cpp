#include <catch/catch.hpp>
#include <pjsettings-jsoncpp.h>
#include <iostream>
#include "SimpleClass.h"

using namespace Json;
using namespace pj;
using namespace pjsettings;

SCENARIO("read json from string")
{
    const char *jsonString = "{\n"
        "   \"intValue\": 14,\n"
        "   \"stringValue\": \"string\",\n"
        "   \"doubleValue\": 2.5,\n"
        "   \"trueBool\":  true,\n"
        "   \"falseBool\": false,\n"
        "   \"simpleClass\": {\n"
        "       \"intValue\": 15,\n"
        "       \"stringValue\": \"string\"\n"
        "   },\n"
        "   \"stringsArray\": [ \"string\", \"other string\" ],\n"
        "   \"boolArray\": [ true, false ],\n"
        "   \"simpleClassArray\": [\n"
        "       { \"intValue\": 16 },\n"
        "       { \"intValue\": 17 }\n"
        "   ],\n"
        "   \"simpleContainer\": {\n"
        "       \"simpleClass\": { \"intValue\": 18 }\n"
        "   },\n"
        "   \"intArray\": [ 19, 20 ],\n"
        "   \"arrayOfStringVectors\": [\n"
        "       [ \"first\", \"second\" ],\n"
        "       [ \"third\", \"fourth\" ]\n"
        "   ]\n"
        "}";

    PjJsonCppDocument doc;
    try
    {
        doc.loadString(jsonString);
    }
    catch (Error &err)
    {
        std::cerr << err.info(true) << std::endl;
        throw;
    }

    SECTION("read simple data types")
    {
        ContainerNode &node = doc.getRootContainer();

        SECTION("read integer")
        {
            int intValue;
            NODE_READ_INT(node, intValue);
            CHECK(14 == intValue);
        }

        SECTION("read double")
        {
            double doubleValue;
            NODE_READ_FLOAT(node, doubleValue);
            CHECK(2.5 == doubleValue);
        }

        SECTION("read string")
        {
            std::string stringValue;
            NODE_READ_STRING(node, stringValue);
            CHECK("string" == stringValue);
        }

        WHEN("read bool")
        {
            THEN("true bool")
            {
                bool trueBool = false;
                NODE_READ_BOOL(node, trueBool);
                CHECK(true == trueBool);
            }

            THEN("false bool")
            {
                bool falseBool = true;
                NODE_READ_BOOL(node, falseBool);
                CHECK(false == falseBool);
            }
        }

        SECTION("read string vector")
        {
            StringVector stringsArray;
            NODE_READ_STRINGV(node, stringsArray);
            REQUIRE(2 == stringsArray.size());
            CHECK("string" == stringsArray[0]);
            CHECK("other string" == stringsArray[1]);
        }
    }

    SECTION("read from array")
    {
        SECTION("read array of objects")
        {
            ContainerNode arrayNode = doc.readArray("simpleClassArray");
            std::vector<SimpleClass> data;
            while (arrayNode.hasUnread())
            {
                SimpleClass obj { "simpleClass" };
                arrayNode.readObject(obj);
                data.push_back(obj);
            }
            REQUIRE(2 == data.size());
            CHECK(data[0].intValue == 16);
            CHECK(data[1].intValue == 17);
        }

        SECTION("read int array")
        {
            ContainerNode arrayNode = doc.readArray("intArray");
            std::vector<int> data;
            while (arrayNode.hasUnread())
            {
                data.push_back(arrayNode.readInt());
            }
            REQUIRE(2 == data.size());
            CHECK(data[0] == 19);
            CHECK(data[1] == 20);
        }

        SECTION("read string array")
        {
            ContainerNode arrayNode = doc.readArray("stringsArray");
            std::vector<std::string> data;
            while (arrayNode.hasUnread())
            {
                data.push_back(arrayNode.readString());
            }
            REQUIRE(2 == data.size());
            CHECK("string" == data[0]);
            CHECK("other string" == data[1]);
        }

        SECTION("read bool array")
        {
            ContainerNode arrayNode = doc.readArray("boolArray");
            std::vector<bool> data;
            while (arrayNode.hasUnread())
            {
                data.push_back(arrayNode.readBool());
            }
            REQUIRE(2 == data.size());
            CHECK(true == data[0]);
            CHECK(false == data[1]);
        }

        SECTION("read StringVector array")
        {
            ContainerNode arrayNode = doc.readArray("arrayOfStringVectors");
            std::vector<StringVector> data;
            while (arrayNode.hasUnread())
            {
                data.push_back(arrayNode.readStringVector());
            }
            REQUIRE(2 == data.size());
            CHECK(data[0].size() == 2);
            CHECK(data[0][0] == "first");
            CHECK(data[0][1] == "second");
            CHECK(data[1].size() == 2);
            CHECK(data[1][0] == "third");
            CHECK(data[1][1] == "fourth");
        }

        SECTION("read array in array")
        {
            ContainerNode arrayNode = doc.readArray("arrayOfStringVectors");
            std::vector<std::string> data;
            while (arrayNode.hasUnread())
            {
                ContainerNode subNode = arrayNode.readArray("vector");
                while (subNode.hasUnread())
                {
                    data.push_back(subNode.readString("add"));
                }
            }
            REQUIRE(4 == data.size());
            CHECK(data[0] == "first");
            CHECK(data[1] == "second");
            CHECK(data[2] == "third");
            CHECK(data[3] == "fourth");
        }
    }

    SECTION("read object")
    {
        SimpleClass simpleClass { "simpleClass" };
        doc.readObject(simpleClass);
        CHECK(simpleClass.intValue == 15);
        CHECK(simpleClass.stringValue == "string");
    }

    SECTION("read container")
    {
        ContainerNode node = doc.readContainer("simpleContainer");
        SimpleClass simpleClass { "simpleClass" };
        node.readObject(simpleClass);
        CHECK(simpleClass.intValue == 18);
    }
}
