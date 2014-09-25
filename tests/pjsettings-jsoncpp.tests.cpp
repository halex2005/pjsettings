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
}
