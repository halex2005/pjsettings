#include <boost/filesystem/operations.hpp>
#include <catch/catch.hpp>
#include <iostream>
#include <pjsettings-jsoncpp.h>
#include "SimpleClass.h"

using namespace Json;
using namespace pj;
using namespace pjsettings;

SCENARIO("read json from string", "[jsoncpp]")
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

    JsonCppDocument doc;
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
            NODE_READ_DOUBLE(node, doubleValue);
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
                SimpleClass obj("simpleClass");
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
        SimpleClass simpleClass("simpleClass");
        doc.readObject(simpleClass);
        CHECK(simpleClass.intValue == 15);
        CHECK(simpleClass.stringValue == "string");
    }

    SECTION("read container")
    {
        ContainerNode node = doc.readContainer("simpleContainer");
        SimpleClass simpleClass("simpleClass");
        node.readObject(simpleClass);
        CHECK(simpleClass.intValue == 18);
    }
}

SCENARIO("jsoncpp read pjsip LogConfig", "[jsoncpp]")
{
    SECTION("read ordered config values")
    {
        const char *jsonString = "{\n"
            "    \"LogConfig\": {\n"
            "        \"msgLogging\": 1,\n"
            "        \"level\": 5,\n"
            "        \"consoleLevel\": 4,\n"
            "        \"decor\": 25328,\n"
            "        \"filename\": \"pjsip.log\",\n"
            "        \"fileFlags\": 0\n"
            "    }\n"
            "}";
        JsonCppDocument doc;
        doc.loadString(jsonString);

        LogConfig config;
        doc.readObject(config);

        CHECK(1 == config.msgLogging);
        CHECK(5 == config.level);
        CHECK(4 == config.consoleLevel);
        CHECK("pjsip.log" == config.filename);
    }

    SECTION("read unordered config values")
    {
        const char *xmlString = "{\n"
            "    \"LogConfig\": {\n"
            "        \"filename\": \"pjsip.log\",\n"
            "        \"level\": 5,\n"
            "        \"consoleLevel\": 4\n"
            "    }\n"
            "}";

        JsonCppDocument doc;
        doc.loadString(xmlString);

        LogConfig config;
        doc.readObject(config);

        CHECK(5 == config.level);
        CHECK(4 == config.consoleLevel);
        CHECK("pjsip.log" == config.filename);
    }
}

SCENARIO("jsoncpp from file", "[jsoncpp]")
{
    const char *filename = "test-config-jsoncpp.json";

    JsonCppDocument doc;
    doc.loadFile(filename);

    LogConfig config;
    doc.readObject(config);

    CHECK(5 == config.level);
    CHECK(4 == config.consoleLevel);
    CHECK("pjsip.log" == config.filename);
}


bool contains_string(JsonCppDocument &doc, const std::string &search)
{
    std::string result = doc.saveString();
    bool expression = result.find(search) != std::string::npos;
    if (!expression)
    {
        std::cout << "substring " << search << " not found in serialized document:" << std::endl << result << std::endl;
    }
    return expression;
}

SCENARIO("jsoncpp to string", "[jsoncpp]")
{
    JsonCppDocument doc(true);

    SECTION("write simple data types")
    {
        ContainerNode &node = doc.getRootContainer();

        SECTION("empty document")
        {
            CHECK(contains_string(doc, "{}"));
        }

        SECTION("write integer")
        {
            int intValue = 14;
            NODE_WRITE_INT(node, intValue);
            CHECK(contains_string(doc, "{\"intValue\":14}"));
        }

        SECTION("write double")
        {
            double doubleValue = 2.5;
            NODE_WRITE_DOUBLE(node, doubleValue);
            CHECK(contains_string(doc, "{\"doubleValue\":2.5}"));
        }

        SECTION("write string")
        {
            std::string stringValue = "string";
            NODE_WRITE_STRING(node, stringValue);
            CHECK(contains_string(doc, "{\"stringValue\":\"string\"}"));
        }

        WHEN("write bool")
        {
            THEN("true bool")
            {
                bool trueBool = true;
                NODE_WRITE_BOOL(node, trueBool);
                CHECK(contains_string(doc, "{\"trueBool\":true}"));
            }

            THEN("false bool")
            {
                bool falseBool = false;
                NODE_WRITE_BOOL(node, falseBool);
                CHECK(contains_string(doc, "{\"falseBool\":false}"));
            }
        }

        SECTION("write string vector")
        {
            StringVector stringsArray;
            stringsArray.push_back("string");
            stringsArray.push_back("other string");

            NODE_WRITE_STRINGV(node, stringsArray);
            CHECK(contains_string(doc, "{\"stringsArray\":[\"string\",\"other string\"]}"));
        }

        SECTION("write container")
        {
            ContainerNode node = doc.writeNewContainer("simpleContainer");

            WHEN("empty container")
            {
                CHECK(contains_string(doc, "{\"simpleContainer\":{}}"));
            }

            WHEN("one attribute")
            {
                node.writeInt("intValue", 21);
                CHECK(contains_string(doc, "{\"simpleContainer\":{\"intValue\":21}}"));
            }

            WHEN("container inside")
            {
                node.writeNewContainer("subContainer");
                CHECK(contains_string(doc, "{\"simpleContainer\":{\"subContainer\":{}}}"));
            }

            WHEN("array inside")
            {
                node.writeNewArray("subArray");
                CHECK(contains_string(doc, "{\"simpleContainer\":{\"subArray\":[]}}"));
            }
        }

        SECTION("write object")
        {
            SimpleClass simpleClass("simpleClass", 15, "string");
            doc.writeObject(simpleClass);
            CHECK(contains_string(doc, "{\"simpleClass\":{\"intValue\":15,\"stringValue\":\"string\"}}"));
        }
    }

    SECTION("write to array")
    {
        ContainerNode arrayNode = doc.writeNewArray("arrayNode");

        SECTION("empty array")
        {
            CHECK(contains_string(doc, "{\"arrayNode\":[]}"));
        }

        SECTION("write int to array")
        {
            arrayNode.writeInt("int", 19);
            CHECK(contains_string(doc, "{\"arrayNode\":[19]}"));
        }

        SECTION("write double to array")
        {
            arrayNode.writeNumber("float", 2.5);
            CHECK(contains_string(doc, "{\"arrayNode\":[2.5]}"));
        }

        SECTION("write string to array")
        {
            arrayNode.writeString("string", "some string");
            CHECK(contains_string(doc, "{\"arrayNode\":[\"some string\"]}"));
        }

        SECTION("write bool to array")
        {
            WHEN("true bool")
            {
                arrayNode.writeBool("boolean", true);
                CHECK(contains_string(doc, "{\"arrayNode\":[true]}"));
            }

            WHEN("false bool")
            {
                arrayNode.writeBool("boolean", false);
                CHECK(contains_string(doc, "{\"arrayNode\":[false]}"));
            }
        }

        SECTION("write StringVector to array")
        {
            StringVector s1;
            s1.push_back("first");
            s1.push_back("second");
            arrayNode.writeStringVector("stringVector", s1);

            CHECK(contains_string(doc, "{\"arrayNode\":[[\"first\",\"second\"]]}"));
        }

        SECTION("write container to array")
        {
            arrayNode.writeNewContainer("simple");
            CHECK(contains_string(doc, "{\"arrayNode\":[{}]}"));
        }

        SECTION("write object to array")
        {
            SimpleClass simpleClass("simple", 16, "string");
            arrayNode.writeObject(simpleClass);
            CHECK(contains_string(doc, "{\"arrayNode\":[{\"intValue\":16,\"stringValue\":\"string\"}]}"));
        }

        SECTION("write array in array")
        {
            ContainerNode subArray = arrayNode.writeNewArray("subArray");
            subArray.writeInt("int", 20);
            CHECK(contains_string(doc, "{\"arrayNode\":[[20]]}"));
        }
    }
}

SCENARIO("jsoncpp write pjsip LogConfig", "[jsoncpp]")
{
    LogConfig config;
    config.filename = "pjsip.log";
    config.consoleLevel = 1;
    config.level = 2;

    JsonCppDocument doc;
    doc.writeObject(config);

    SECTION("write to string")
    {
        size_t npos = std::string::npos;
        CHECK(contains_string(doc, "\"LogConfig\" :"));
        CHECK(contains_string(doc, "\"filename\" : \"pjsip.log\""));
        CHECK(contains_string(doc, "\"consoleLevel\" : 1"));
        CHECK(contains_string(doc, "\"level\" : 2"));
    }

    SECTION("write to file")
    {
        using namespace boost::filesystem;
        char const *filename = "test-save-LogConfig-to-file.json";
        remove(filename);
        CHECK(!exists(filename));
        doc.saveFile(filename);
        REQUIRE(exists(filename));
    }
}
