#include <catch/catch.hpp>
#include <pjsettings-pugixml.h>
#include <pjsua2/endpoint.hpp>
#include <iostream>
#include "SimpleClass.h"

using namespace pj;
using namespace pjsettings;

SCENARIO("pugixml from string")
{
    const char *xmlString = ""
        "<?xml version=\"1.0\"?>\n"
        "<root intValue=\"14\"\n"
        "      stringValue=\"string\"\n"
        "      doubleValue=\"2.5\"\n"
        "      trueBool=\"true\"\n"
        "      falseBool=\"false\"\n"
        "      >\n"
        "   <simpleClass intValue=\"15\"\n"
        "                stringValue=\"string\" />\n"
        "   <stringsArray>\n"
        "       <add>string</add>\n"
        "       <add>other string</add>\n"
        "   </stringsArray>\n"
        "   <simpleClassArray>\n"
        "       <add intValue=\"16\" />\n"
        "       <add intValue=\"17\" />\n"
        "   </simpleClassArray>\n"
        "   <simpleContainer>\n"
        "       <simpleClass intValue=\"18\" />\n"
        "   </simpleContainer>\n"
        "   <intArray>\n"
        "       <add>19</add>\n"
        "       <add>20</add>\n"
        "   </intArray>\n"
        "   <arrayOfStringVectors>\n"
        "       <vector>\n"
        "           <add>first</add>\n"
        "           <add>second</add>\n"
        "       </vector>\n"
        "       <vector>\n"
        "           <add>third</add>\n"
        "           <add>fourth</add>\n"
        "       </vector>\n"
        "   </arrayOfStringVectors>\n"
        "</root>";
    PjPugixmlDocument doc;
    try
    {
        doc.loadString(xmlString);
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

    SECTION("read object")
    {
        SimpleClass simpleClass { "simpleClass" };
        doc.readObject(simpleClass);
        CHECK(simpleClass.intValue == 15);
        CHECK(simpleClass.stringValue == "string");
    }

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

        //TODO: test read string vector from array
        //TODO: test read simple data from array
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

    SECTION("read container")
    {
        ContainerNode node = doc.readContainer("simpleContainer");
        SimpleClass simpleClass { "simpleClass" };
        node.readObject(simpleClass);
        CHECK(simpleClass.intValue == 18);
    }
}

SCENARIO("pugixml read pjsip LogConfig")
{
    SECTION("read ordered config values")
    {
        const char *xmlString = ""
            "<root>\n"
            "    <LogConfig msgLogging=\"1\"\n"
            "        level=\"5\"\n"
            "        consoleLevel=\"4\"\n"
            "        decor=\"25328\"\n"
            "        filename=\"pjsip.log\"\n"
            "        fileFlags=\"0\"\n"
            "        >"
            "    </LogConfig>"
            "</root>";
        PjPugixmlDocument doc;
        doc.loadString(xmlString);

        LogConfig config;
        doc.readObject(config);

        CHECK(1 == config.msgLogging);
        CHECK(5 == config.level);
        CHECK(4 == config.consoleLevel);
        CHECK("pjsip.log" == config.filename);
    }

    SECTION("read unordered config values")
    {
        const char *xmlString = "<root>\n"
            "    <LogConfig\n"
            "        filename=\"pjsip.log\"\n"
            "        level=\"5\"\n"
            "        consoleLevel=\"4\"\n"
            "        >"
            "    </LogConfig>\n"
            "</root>";

        PjPugixmlDocument doc;
        doc.loadString(xmlString);

        LogConfig config;
        doc.readObject(config);

        CHECK(5 == config.level);
        CHECK(4 == config.consoleLevel);
        CHECK("pjsip.log" == config.filename);
    }
}

SCENARIO("pugixml from file")
{
    const char *filename = "test-config-pugixml.xml";

    PjPugixmlDocument doc;
    doc.loadFile(filename);

    LogConfig config;
    doc.readObject(config);

    CHECK(5 == config.level);
    CHECK(4 == config.consoleLevel);
    CHECK("pjsip.log" == config.filename);
}
