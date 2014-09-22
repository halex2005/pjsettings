CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

include_directories(.)

add_library(pjsettings-json
    pjsettings-jsoncpp.h
    pjsettings-jsoncpp.cpp
    json.h
    json-forwards.h
    jsoncpp.cpp
)

add_library(pjsettings-pugixml
    pjsettings-pugixml.h
    pjsettings-pugixml.cpp
    pugixml.hpp
    pugixml.cpp
)

add_subdirectory(tests)