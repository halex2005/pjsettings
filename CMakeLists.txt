CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

include_directories(.)

set(pjsettings-json
    pjsettings-jsoncpp.h
    pjsettings-jsoncpp.cpp
    json.h
    json-forwards.h
    jsoncpp.cpp
)
source_group(jsoncpp FILES ${pjsettings-json})

set(pjsettings-pugixml
    pjsettings-pugixml.h
    pjsettings-pugixml.cpp
    pugixml.hpp
    pugixml.cpp
)
source_group(pugixml FILES ${pjsettings-pugixml})

set(pjsettings-persistent
    persistent.hpp
    persistent.cpp
    types.hpp
    types.cpp
)
source_group(persistent FILES ${pjsettings-persistent})

add_library(pjsettings ${pjsettings-json} ${pjsettings-pugixml} ${pjsettings-persistent})

add_subdirectory(tests)
