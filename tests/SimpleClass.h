#ifndef __SIMPLE_CLASS_FOR_TESTS_H__
#define __SIMPLE_CLASS_FOR_TESTS_H__

#include "../persistent.hpp"

class SimpleClass : public pj::PersistentObject
{
public:
    SimpleClass(const std::string &elementName)
        : _elementName(elementName)
    {
    }

    SimpleClass(const std::string &elementName, int i, std::string s = "")
        : _elementName(elementName)
        , intValue(i)
        , stringValue(s)
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
        pj::ContainerNode this_node = node.writeNewContainer(_elementName.c_str());
        NODE_WRITE_INT(this_node, intValue);
        NODE_WRITE_STRING(this_node, stringValue);
    }

    int intValue;
    std::string stringValue;
private:
    std::string _elementName;
};

namespace pj
{

    class LogConfig : public pj::PersistentObject
    {
    public:
        LogConfig()
            : msgLogging(PJ_TRUE)
            , level(5)
            , consoleLevel(4)
            , decor()
            , filename()
            , fileFlags()
        {}

        virtual void readObject(const ContainerNode &node) throw(Error)
        {
            ContainerNode this_node = node.readContainer("LogConfig");

            NODE_READ_UNSIGNED( this_node, msgLogging);
            NODE_READ_UNSIGNED( this_node, level);
            NODE_READ_UNSIGNED( this_node, consoleLevel);
            NODE_READ_UNSIGNED( this_node, decor);
            NODE_READ_STRING  ( this_node, filename);
            NODE_READ_UNSIGNED( this_node, fileFlags);
        }

        virtual void writeObject(ContainerNode &node) const throw(Error)
        {
            ContainerNode this_node = node.writeNewContainer("LogConfig");

            NODE_WRITE_UNSIGNED( this_node, msgLogging);
            NODE_WRITE_UNSIGNED( this_node, level);
            NODE_WRITE_UNSIGNED( this_node, consoleLevel);
            NODE_WRITE_UNSIGNED( this_node, decor);
            NODE_WRITE_STRING  ( this_node, filename);
            NODE_WRITE_UNSIGNED( this_node, fileFlags);
        }

        /** Log incoming and outgoing SIP message? Yes!  */
        unsigned        msgLogging;

        /** Input verbosity level. Value 5 is reasonable. */
        unsigned        level;

        /** Verbosity level for console. Value 4 is reasonable. */
        unsigned        consoleLevel;

        /** Log decoration. */
        unsigned        decor;

        /** Optional log filename if app wishes the library to write to log file.
         */
        string      filename;

        /**
         * Additional flags to be given to pj_file_open() when opening
         * the log file. By default, the flag is PJ_O_WRONLY. Application
         * may set PJ_O_APPEND here so that logs are appended to existing
         * file instead of overwriting it.
         *
         * Default is 0.
         */
        unsigned        fileFlags;
    };

}
#endif
