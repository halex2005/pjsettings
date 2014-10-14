#include <boost/filesystem/operations.hpp>
#include <catch/catch.hpp>
#include <pjsettings-pugixml.h>
#include <iostream>
#include "SimpleClass.h"

using namespace pj;
using namespace pjsettings;

SCENARIO("pugixml from string", "[pugixml]")
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
        "   <boolArray>"
        "       <add>true</add>"
        "       <add>false</add>"
        "   </boolArray>\n"
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
    PugixmlDocument doc;
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

SCENARIO("pugixml read pjsip LogConfig", "[pugixml]")
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
        PugixmlDocument doc;
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

        PugixmlDocument doc;
        doc.loadString(xmlString);

        LogConfig config;
        doc.readObject(config);

        CHECK(5 == config.level);
        CHECK(4 == config.consoleLevel);
        CHECK("pjsip.log" == config.filename);
    }
}

SCENARIO("pugixml from file", "[pugixml]")
{
    const char *filename = "test-config-pugixml.xml";

    PugixmlDocument doc;
    doc.loadFile(filename);

    LogConfig config;
    doc.readObject(config);

    CHECK(5 == config.level);
    CHECK(4 == config.consoleLevel);
    CHECK("pjsip.log" == config.filename);
}


bool contains_string(PugixmlDocument &doc, const std::string &search)
{
    std::string result = doc.saveString();
    bool expression = result.find(search) != std::string::npos;
    if (!expression)
    {
        std::cout << "substring " << search << " not found in serialized document:" << std::endl << result << std::endl;
    }
    return expression;
}

SCENARIO("pugixml to string", "[pugixml]")
{
    PugixmlDocument doc;

    SECTION("write simple data types")
    {
        ContainerNode &node = doc.getRootContainer();

        SECTION("write integer")
        {
            int intValue = 14;
            NODE_WRITE_INT(node, intValue);
            CHECK(contains_string(doc, "intValue=\"14\""));
        }

        SECTION("write double")
        {
            double doubleValue = 2.5;
            NODE_WRITE_FLOAT(node, doubleValue);
            CHECK(contains_string(doc, "doubleValue=\"2.5\""));
        }

        SECTION("write string")
        {
            std::string stringValue = "string";
            NODE_WRITE_STRING(node, stringValue);
            CHECK(contains_string(doc, "stringValue=\"string\""));
        }

        WHEN("write bool")
        {
            THEN("true bool")
            {
                bool trueBool = true;
                NODE_WRITE_BOOL(node, trueBool);
                CHECK(contains_string(doc, "trueBool=\"true\""));
            }

            THEN("false bool")
            {
                bool falseBool = false;
                NODE_WRITE_BOOL(node, falseBool);
                CHECK(contains_string(doc, "falseBool=\"false\""));
            }
        }

        SECTION("write string vector")
        {
            StringVector stringsArray;
            stringsArray.push_back("string");
            stringsArray.push_back("other string");

            NODE_WRITE_STRINGV(node, stringsArray);
            CHECK(contains_string(doc, "<stringsArray>"));
            CHECK(contains_string(doc, "<item>string</item>"));
            CHECK(contains_string(doc, "<item>other string</item>"));
            CHECK(contains_string(doc, "</stringsArray>"));
        }

        SECTION("write container")
        {
            ContainerNode node = doc.writeNewContainer("simpleContainer");

            WHEN("empty container")
            {
                CHECK(contains_string(doc, "<simpleContainer />"));
            }

            WHEN("one attribute")
            {
                node.writeInt("intValue", 21);
                CHECK(contains_string(doc, "<simpleContainer intValue=\"21\" />"));
            }

            WHEN("container inside")
            {
                node.writeNewContainer("subContainer");
                CHECK(contains_string(doc, "<simpleContainer>"));
                CHECK(contains_string(doc, "<subContainer />"));
                CHECK(contains_string(doc, "</simpleContainer>"));
            }

            WHEN("array inside")
            {
                node.writeNewArray("subArray");
                CHECK(contains_string(doc, "<simpleContainer>"));
                CHECK(contains_string(doc, "<subArray />"));
                CHECK(contains_string(doc, "</simpleContainer>"));
            }
        }

        SECTION("write object")
        {
            SimpleClass simpleClass("simpleClass", 15, "string");
            doc.writeObject(simpleClass);
            CHECK(contains_string(doc, "<simpleClass"));
            CHECK(contains_string(doc, "intValue=\"15\""));
            CHECK(contains_string(doc, "stringValue=\"string\""));
            CHECK(contains_string(doc, "/>"));
        }
    }

    SECTION("write to array")
    {
        ContainerNode arrayNode = doc.writeNewArray("arrayNode");

        SECTION("empty array")
        {
            CHECK(contains_string(doc, "<arrayNode />"));
        }

        SECTION("write int to array")
        {
            arrayNode.writeInt("int", 19);
            CHECK(contains_string(doc, "<item>19</item>"));
        }

        SECTION("write double to array")
        {
            arrayNode.writeNumber("float", 2.5);
            CHECK(contains_string(doc, "<item>2.5</item>"));
        }

        SECTION("write string to array")
        {
            arrayNode.writeString("string", "some string");
            CHECK(contains_string(doc, "<item>some string</item>"));
        }

        SECTION("write bool to array")
        {
            WHEN("true bool")
            {
                arrayNode.writeBool("boolean", true);
                CHECK(contains_string(doc, "<item>true</item>"));
            }

            WHEN("false bool")
            {
                arrayNode.writeBool("boolean", false);
                CHECK(contains_string(doc, "<item>false</item>"));
            }
        }

        SECTION("write StringVector to array")
        {
            StringVector s1;
            s1.push_back("first");
            s1.push_back("second");
            arrayNode.writeStringVector("stringVector", s1);

            CHECK(contains_string(doc, "<stringVector>"));
            CHECK(contains_string(doc, "</stringVector>"));
            CHECK(contains_string(doc, "<item>first</item>"));
            CHECK(contains_string(doc, "<item>second</item>"));
        }

        SECTION("write container to array")
        {
            arrayNode.writeNewContainer("simple");
            CHECK(contains_string(doc, "<simple />"));
        }

        SECTION("write object to array")
        {
            SimpleClass simpleClass("simple", 16);
            arrayNode.writeObject(simpleClass);
            CHECK(contains_string(doc, "<simple intValue=\"16\""));
        }

        SECTION("write array in array")
        {
            ContainerNode subArray = arrayNode.writeNewArray("subArray");
            subArray.writeInt("int", 20);
            CHECK(contains_string(doc, "<subArray>"));
            CHECK(contains_string(doc, "</subArray>"));
            CHECK(contains_string(doc, "<item>20</item>"));
        }

        SECTION("write multiple values to array")
        {
            std::vector<int> values;
            values.push_back(1);
            values.push_back(2);
            for (int i = 0; i < values.size(); ++i)
            {
                NODE_WRITE_INT(arrayNode, values[i]);
            }
            CHECK(contains_string(doc, "<item>1</item>"));
            CHECK(contains_string(doc, "<item>2</item>"));
        }

        SECTION("write multiple objects to array")
        {
            std::vector<SimpleClass> values;
            values.push_back(SimpleClass("simple", 1));
            values.push_back(SimpleClass("simple", 2));
            for (int i = 0; i < values.size(); ++i)
            {
                NODE_WRITE_OBJ(arrayNode, values[i]);
            }
            CHECK(contains_string(doc, "<simple intValue=\"1\""));
            CHECK(contains_string(doc, "<simple intValue=\"2\""));
        }
    }
}

SCENARIO("pugixml write pjsip LogConfig", "[pugixml]")
{
    LogConfig config;
    config.filename = "pjsip.log";
    config.consoleLevel = 1;
    config.level = 2;

    PugixmlDocument doc(pugi::format_indent);
    doc.writeObject(config);

    SECTION("write to string")
    {
        std::string savedString = doc.saveString();

        size_t npos = std::string::npos;
        CHECK(savedString.find("<root>") != npos);
        CHECK(savedString.find("</root>") != npos);
        CHECK(savedString.find("LogConfig") != npos);
        CHECK(savedString.find("filename=\"pjsip.log\"") != npos);
        CHECK(savedString.find("consoleLevel=\"1\"") != npos);
        CHECK(savedString.find("level=\"2\"") != npos);
        std::cout << savedString << std::endl;
    }

    SECTION("write to file")
    {
        using namespace boost::filesystem;
        char const *filename = "test-save-LogConfig-to-file.xml";
        remove(filename);
        CHECK(!exists(filename));
        doc.saveFile(filename);
        REQUIRE(exists(filename));
    }
}
