#ifndef __SIMPLE_CLASS_FOR_TESTS_H__
#define __SIMPLE_CLASS_FOR_TESTS_H__

#include <pjsua2/persistent.hpp>

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

#endif