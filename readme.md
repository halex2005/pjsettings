PJSettings
==========

[![Build Status](https://travis-ci.org/halex2005/pjsettings.svg?branch=master)](https://travis-ci.org/halex2005/pjsettings)

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

Copyright (C) 2014, by [halex2005](mailto:akharlov@gmail.com)
Report bugs and download new versions at https://github.com/halex2005/pjsettings

Note that PJSettings library is built on to of PJSUA2 subset of PJSIP library.
Please, [look at PJSIP license options](http://www.pjsip.org/licensing.htm) if you would use PJSettings library.

[![PayPal donate button](http://img.shields.io/paypal/donate.png?color=yellow)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=DNYQXBLEV475C "Donate once-off to this project using Paypal")
[![Gratipay donate button](http://img.shields.io/gratipay/halex2005.svg)](https://gratipay.com/halex2005/ "Donate weekly to this project using Gratipay")