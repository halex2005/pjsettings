PJSettings
==========

Platform | Linux | Windows
---------|-------|--------
Build status | [![Build Status](https://travis-ci.org/halex2005/pjsettings.svg?branch=master)](https://travis-ci.org/halex2005/pjsettings) | [![Build status](https://ci.appveyor.com/api/projects/status/ehy5ody4geoirtgd)](https://ci.appveyor.com/project/halex2005/pjsettings)

## modified-persistent-api branch

**modified-persistent-api branch** of PJSettings has rewritten PJSIP persistent classes to support preserving default values on read.

Main changes:

- double as main type for readNumber() functions family
- support for preserving default values on read when values not exists in confuguration (see `void readNumber(name, value)` functions)
- `NODE_READ_*` macros will preserve default values
- You can use this branch without linking with PJSIP libraries (all dependencies are in place).

Restrictions:

- You cannot use this branch to read PJSIP configuration classes such as pj::LogConfig and pj::EpConfig. This branch is designed to use without linking with PJSIP libraries, in projects that not uses PJSIP altogether.

## PJSettings

PJSettings is a small library that extends a PJSIP persistent document interfaces to support:

- read/write xml document non-serially, based on [pugixml backend](http://pugixml.org/)
- read/write json document non-serially, based on [jsoncpp backend](https://github.com/open-source-parsers/jsoncpp)

Inspite PJSUA2 already has implementation for read/write json documents,
[PJSUA2 pj::JsonDocument](http://www.pjsip.org/docs/book-latest/html/reference.html#json-hpp)
is read and written serially, hence the order of reading must be
the same as the order of writing.

This library provides classes capable reading of documents in non-serial way.

For example, you can read pj::LogConfig object from this json document in usual serial way:

```json
{
    "LogConfig": {
        "msgLogging": 1,
        "level": 5,
        "consoleLevel": 4,
        "decor": 25328,
        "filename": "pjsip.log",
        "fileFlags": 0
    }
}
```

or you can have just small part of fields, and can reorder them on your own:

```json
{
    "LogConfig": {
        "filename": "pjsip.log",
        "level": 5,
        "consoleLevel": 4
    }
}
```

Same xml document will look as

```xml
<?xml version="1.0"?>
<root>
    <LogConfig filename="pjsip.log"
               level="5"
               consolelevel="4" />
</root>
```

For more details, please read

- [jsoncpp pjsettings features](pjsettings-jsoncpp.md)
- [pugixml pjsettings features](pjsettings-pugixml.md)

Third-party libraries
---------------------

This library use great libraries to work well (in alphabetical order):

- [catch by Two Blue Cubes Ltd.](https://github.com/philsquared/Catch) for unit testing
- [jsoncpp by Baptiste Lepilleur](https://github.com/open-source-parsers/jsoncpp) for json backend
- [pjsip](http://www.pjsip.org/) by Benny Prijono and [Teluu Inc.](http://www.teluu.com) for persistent APIs
- [pugixml by Arseny Kapoulkine](http://pugixml.org/) for xml backend

License
-------

PJSettings library is distributed under [MIT license](COPYING)

Copyright (C) 2014, by [halex2005](mailto:akharlov@gmail.com) <br/>
Report bugs and download new versions at https://github.com/halex2005/pjsettings

Note that PJSettings library is built on to of PJSUA2 subset of PJSIP library.
Please, [look at PJSIP license options](http://www.pjsip.org/licensing.htm) if you would use PJSettings library.

[![PayPal donate button](http://img.shields.io/paypal/donate.png?color=yellow)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=DNYQXBLEV475C "Donate once-off to this project using Paypal")
[![Gratipay donate button](http://img.shields.io/gratipay/halex2005.svg)](https://gratipay.com/halex2005/ "Donate weekly to this project using Gratipay")