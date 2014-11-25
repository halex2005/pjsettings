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

        find_path(PJSIP_PJLIB_INCLUDE_DIR NAMES "pjlib.h" PATHS "${PJSIP_ROOT_DIR}/pjlib/include")
        find_path(PJSIP_PJLIB_UTIL_INCLUDE_DIR NAMES "pjlib-util.h" PATHS "${PJSIP_ROOT_DIR}/pjlib-util/include")
        find_path(PJSIP_PJMEDIA_INCLUDE_DIR NAMES "pjmedia.h" PATHS "${PJSIP_ROOT_DIR}/pjmedia/include")
        find_path(PJSIP_PJNATH_INCLUDE_DIR NAMES "pjnath.h" PATHS "${PJSIP_ROOT_DIR}/pjnath/include")
        find_path(PJSIP_PJSIP_INCLUDE_DIR NAMES "pjsip.h" PATHS "${PJSIP_ROOT_DIR}/pjsip/include")
        set (PJSIP_INCLUDE_DIR
            ${PJSIP_PJLIB_INCLUDE_DIR}
            ${PJSIP_PJLIB_UTIL_INCLUDE_DIR}
            ${PJSIP_PJMEDIA_INCLUDE_DIR}
            ${PJSIP_PJNATH_INCLUDE_DIR}
            ${PJSIP_PJSIP_INCLUDE_DIR}
        )
        include_directories(${PJSIP_INCLUDE_DIR})

        set (PJSIP_LIBRARIES "libpjproject-\$(Platform)-\$(PlatformToolset)-\$(Configuration)-Dynamic.lib")
        set (PJSIP_STATIC_LIBRARIES ${PJSIP_LIBRARIES} Ws2_32.lib)
        set (PJSIP_LIBRARY_DIR "${PJSIP_ROOT_DIR}/lib")
        link_directories(${PJSIP_LIBRARY_DIR})
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
