XML-based configuration in pjsettings
=====================================

You can use class pjsettings::PugixmlDocument to read xml configuration:

```c++
pjsettings::PugixmlDocument doc;
try
{
    doc.loadFile("config.xml");
}
catch (pj::Error &err)
{
    std::cerr << err.info(true) << std::endl;
    throw;
}
```

After the document is successfully loaded, it is ready to read persistent objects.
You can use document in almost all cases where you can use [pj::JsonDocument class](http://www.pjsip.org/docs/book-latest/html/intro_pjsua2.html#objects-persistence).
For example, you can read [pj::LogConfig (log configuration class)](http://www.pjsip.org/docs/book-latest/html/endpoint.html#endpoint-configurations):

```c++
pj::EpConfig config;
doc.readObject(config);
```

from xml document:

```xml
<?xml version="1.0"?>
<root>
    <LogConfig filename="pjsip.log"
               level="5"
               consolelevel="4" />
</root>
```

Features for pugixml backend
----------------------------

- attribute names and container names can be in any order in document
- simple data is stored in xml attributes
    + bool value is true if text starts with one of character: "1", "t", "T", "y", "Y", otherwise bool value is false
    + all numbers is read as double (can't read very large 64-bit integers without precision loss)
- simple data in arrays is storead as xml-element, xml-text is the value
- objects is stored as xml-elements
- arrays is stored as xml-elements, names for sub-elements is ignored, can be whatever
- string vector is stored as array of strings

Next sections will explain this features in more details.

### Simple data (bool, int, number, string)

Simple data in object is stored as xml attributes:

```xml
<root>
    <LogConfig level="4" filename="pjsip.log"/>
</root>
```

You can read/write simple data:

```c++
// read
pj::ContainerNode node = doc.readContainer("LogConfig");
NODE_READ_INT(node, level);
NODE_READ_STRING(node, filename);

// write
pj::ContainerNode node = doc.writeNewContainer("LogCnfig");
NODE_WRITE_INT(node, level);
NODE_WRITE_STRING(node, filename);
```

### Objects

Objects are stored like xml element with specified name:

```xml
<root>
    <object>
        <subobject />
    </object>
</root>

```

If you have custom persistent object like, you can read/write objects:

```c++
class CustomPersistentObject : public pj::PersistentObject
{
    virtual void readObject(const ContainerNode &node) throw(Error)
    {
        pj::ContainerNode object = doc.readContainer("object");
        pj::ContainerNode subobject = doc.readContainer("subobject");
    }

    virtual void writeObject(ContainerNode &node) const throw(Error)
    {
        pj::ContainerNode object = doc.writeNewContainer("object");
        pj::ContainerNode subobject = doc.writeNewContainer("subobject");
    }
};

CustomPersistentObject someObject;

// read
NODE_READ_OBJ(doc, someObject);

// write
NODE_WRITE_OBJ(doc, someObject);
```

It is recommended way to read container with real object name (like "object" in example) in overriden pj::PersistentObject::readObject() function, and to write container with real object name in overriden pj::PersistentObject::writeObject() function.

### Data in array

Simple data and objects in array is stored as `<item>` xml element with text inside it on write (on read xml element name is ignored, can be whatever):

```xml
<root>
    <array>
        <item>1</item>
        <item>2</item>
    </array>
    <LogConfigs>
        <add level="4" filename="pjsip.org" />
        <remove level="5" />
    </LogConfigs>
</root>
```

You can read from/write to array:

```c++
std::vector<int> arrayItems;
std::vector<pj::LogConfig> logConfigItems;

// read ints
pj::ContainerNode array = doc.readArray("array");
while (array.hasUnread())
{
    int arrayItem;
    NODE_READ_INT(array, arrayItem);
    arrayItems.push_back(arrayItem);
}
CHECK(2 = arrayItems.size());

// read objects - xml element names is ignored!
pj::ContainerNode logsNode = doc.readArray("LogConfigs");
while (array.hasUnread())
{
    LogConfig item;
    NODE_READ_OBJ(logsNode, item);
    logConfigItems.push_back(item);
}
CHECK(2 = arrayItems.size());

// write
pj::ContainerNode array = doc.writeNewArray("array");
for (int i = 0; i < arrayItems; ++i)
{
    NODE_WRITE_INT(array, arrayItems[i]);
}

// write objects
pj::ContainerNode logsNode = doc.writeNewArray("LogConfigs");
for (int i = 0; i < logConfigItems.size(); ++i)
{
    NODE_WRITE_OBJ(logsNode, logConfigItems[i]);
}
```

### String vectors

String vectors are stored like array of strings:

```xml
<root>
    <array>
        <item>string one</item>
        <item>string two</item>
    </array>
</root>
```

You can read/write string vector:

```c++
pj::StringVector array;

// read
NODE_READ_STRINGV(doc, array);

// write
NODE_WRITE_STRINGV(doc, array);
```
