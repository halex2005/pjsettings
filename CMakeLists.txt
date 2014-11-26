CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

option(PJSETTINGS_USE_EXTERNAL_JSONCPP OFF)
option(PJSETTINGS_USE_EXTERNAL_PUGIXML OFF)
option(PJSETTINGS_NO_TESTS OFF)

project(pjsettings LANGUAGES CXX)
include_directories(.)

if (NOT PJSIP_FOUND)

    if (UNIX)
        add_definitions(-fpermissive)

        find_package(PkgConfig REQUIRED)
        pkg_check_modules(PJSIP libpjproject REQUIRED)
        if (NOT PJSIP_FOUND)
            message(SEND_ERROR "PJSIP libraries not found")
            return()
        endif()
    elseif(WIN32)
        add_definitions(-DPJ_WIN32=1)
        if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
            add_definitions(-DPJ_M_X86_64)
        else()
            add_definitions(-DPJ_M_I386)
        endif()

        set (PJSIP_ROOT_DIR $ENV{PJSIP_ROOT})

        set (PJSIP_INCLUDE_DIR ${PJSIP_ROOT_DIR}/include)
        include_directories(${PJSIP_INCLUDE_DIR})

        set (PJSIP_LIBRARY_DIR ${PJSIP_ROOT_DIR}/lib)
        link_directories(${PJSIP_LIBRARY_DIR})

        set (PJSIP_LIBRARIES "libpjproject")
        set (PJSIP_PLATFORM_LIBRARIES "Ws2_32;ole32;wsock32;dsound")
        set (PJSIP_STATIC_LIBRARIES ${PJSIP_LIBRARIES} ${PJSIP_PLATFORM_LIBRARIES})
    endif()
endif()

set(pjsettings-json
    pjsettings-jsoncpp.h
    pjsettings-jsoncpp.cpp
)
if (NOT PJSETTINGS_USE_EXTERNAL_JSONCPP)
    list(APPEND pjsettings-json
        json.h
        json-forwards.h
        jsoncpp.cpp
    )
endif()
source_group(jsoncpp FILES ${pjsettings-json})

set(pjsettings-pugixml
    pjsettings-pugixml.h
    pjsettings-pugixml.cpp
)
if (NOT PJSETTINGS_USE_EXTERNAL_PUGIXML)
    list(APPEND pjsettings-pugixml
        pugiconfig.hpp
        pugixml.hpp
        pugixml.cpp
    )
endif()
source_group(pugixml FILES ${pjsettings-pugixml})

add_library(pjsettings ${pjsettings-json} ${pjsettings-pugixml})

if (NOT PJSETTINGS_NO_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
