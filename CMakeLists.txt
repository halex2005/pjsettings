CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

include_directories(.)

if (UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PJSIP libpjproject REQUIRED)
    if (NOT PJSIP_FOUND)
        message(SEND_ERROR "PJSIP libraries not found")
        return()
    endif()
    include_directories(${PJSIP_INCLUDE_DIRS})
endif()

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

add_library(pjsettings ${pjsettings-json} ${pjsettings-pugixml})

add_subdirectory(tests)
