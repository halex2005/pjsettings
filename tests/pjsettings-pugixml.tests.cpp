#include <catch/catch.hpp>
#include <pjsettings-pugixml.h>
#include <pjsua2/endpoint.hpp>

using namespace pj;
using namespace pjsettings;

class SimpleClass : public pj::PersistentObject
{
public:
    SimpleClass(const std::string &elementName)
        : _elementName(elementName)
    {
    }

    virtual void readObject(const pj::ContainerNode &node) throw(pj::Error)
    {
        pj::ContainerNode this_node = node.readContainer(_elementName.c_str());
        NODE_READ_INT(this_node, intValue);
        NODE_READ_STRING(this_node, stringValue);
    }


    virtual void writeObject(pj::ContainerNode &node) const throw(pj::Error)
    {
        pj::ContainerNode this_node = node.readContainer(_elementName.c_str());
        NODE_WRITE_INT(this_node, intValue);
        NODE_WRITE_STRING(this_node, stringValue);
    }

    int intValue;
    std::string stringValue;
private:
    std::string _elementName;
};

SCENARIO("pugixml from string")
{
    const char *xmlString = "\
<?xml version=\"1.0\"?>\
<root intValue=\"14\" stringValue=\"string\" doubleValue=\"2.5\" trueBool=\"true\" falseBool=\"false\">\
<simpleClass intValue=\"15\" stringValue=\"string\" />\
<stringsArray>\
  <add>string</add>\
  <add>other string</add>\
</stringsArray>\
<simpleClassArray>\
  <add intValue=\"16\" />\
  <add intValue=\"17\" />\
</simpleClassArray>\
<simpleContainer>\
  <simpleClass intValue=\"18\" />\
</simpleContainer>\
</root>";
    PjPugixmlDocument doc;
    doc.loadString(xmlString);

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

    SECTION("read array")
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

    SECTION("read container")
    {
        ContainerNode node = doc.readContainer("simpleContainer");
        SimpleClass simpleClass { "simpleClass" };
        node.readObject(simpleClass);
        CHECK(simpleClass.intValue == 18);
    }

}
