PJSettings
==========

PJSettings is small library that extends a PJSIP persistent document interfaces to support:

- read/write xml document non-serially, based on [pugixml backend](http://pugixml.org/)
- read/write json document non-serially, based on [jsoncpp backend](https://github.com/open-source-parsers/jsoncpp)

Inspite PJSUA2 already has implementation for read/write json documents,
PJSUA2 document is read and written serially, hence the order of reading
must be the same as the order of writing.

This library provides classes capable reading of documents in non-serial way.

For example, if you can have this json document:

```json
// example of full json-representation for class pj::LogConfig
{
    "LogConfig": {
        "msgLogging": 1,
        "level": 5,
        "consoleLevel": 4,
        "decor": 25328,
        "filename": "pjsip.log",
        "fileFlags": 0
    },
}
```

or you can have just small part of fields, and can reorder them on your own:

```json
// example of small part of fields with reordering
{
    "LogConfig": {
        "filename": "pjsip.log",
        "level": 5,
        "consoleLevel": 4,
    },
}
```

Third-party libraries
---------------------

This library use this great libraries (in alphabetical order):

- [catch](https://github.com/philsquared/Catch) by Two Blue Cubes Ltd. for unit testing
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp) by Baptiste Lepilleur for json backend
- [pjsip](http://www.pjsip.org/) by Benny Prijono and [Teluu Inc.](http://www.teluu.com) for persistent APIs
- [pugixml](http://pugixml.org/) by Arseny Kapoulkine (MIT license) for xml backend

Usage
-----



License
-------

This library is distributed with same license as for [PJSIP](http://www.pjsip.org/licensing.htm):

- [GPL v2](COPYING) or above.
- Alternatively, if you bought [proprietary PJSIP license from Teluu Inc.](http://www.pjsip.org/licensing.htm), you can use this library with conditions of that proprietary license. Please, ask [licensing@teluu.com](licensing@teluu.com) for license information (I am not an employee of Teluu Inc.).

[![PayPal donate button](http://img.shields.io/paypal/donate.png?color=yellow)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=DNYQXBLEV475C "Donate once-off to this project using Paypal")
[![Gratipay donate button](http://img.shields.io/gratipay/halex2005.svg)](https://gratipay.com/halex2005/ "Donate weekly to this project using Gratipay")