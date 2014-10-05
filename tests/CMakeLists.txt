CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

include_directories(.)

###################################
# boost
add_definitions(-DBOOST_SYSTEM_NO_DEPRECATED)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED ON)
find_package(Boost 1.45.0 REQUIRED
    COMPONENTS
        filesystem
        system
    )
if(NOT Boost_FOUND)
    message(SEND_ERROR "Failed to find boost libraries")
    return()
else()
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif()
###################################

add_executable(test-pjsettings
    main.cpp
    pjsettings-jsoncpp.tests.cpp
    pjsettings-pugixml.tests.cpp
    SimpleClass.h
    test-config-jsoncpp.json
    test-config-pugixml.xml
)
target_link_libraries(test-pjsettings pjsettings ${Boost_LIBRARIES})
if (UNIX)
    add_definitions(-fpermissive)

    ###################################
    # PJSIP
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PJSIP libpjproject REQUIRED)
    if (NOT PJSIP_FOUND)
        message(SEND_ERROR "PJSIP libraries not found")
        return()
    endif()
    ###################################
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

    set (PJSIP_LIBRARY_DIR "${PJSIP_ROOT_DIR}/lib")
    set (PJSIP_LIBRARIES "libpjproject-\$(Platform)-\$(PlatformToolset)-\$(Configuration)-Dynamic.lib")
    set (PJSIP_STATIC_LIBRARIES ${PJSIP_LIBRARIES} Ws2_32.lib)
    link_directories(${PJSIP_LIBRARY_DIR})
endif()

target_link_libraries(test-pjsettings ${PJSIP_STATIC_LIBRARIES})

add_custom_command(
    TARGET test-pjsettings PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/test-config-pugixml.xml $<TARGET_FILE_DIR:test-pjsettings>)

add_custom_command(
    TARGET test-pjsettings PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/test-config-jsoncpp.json $<TARGET_FILE_DIR:test-pjsettings>)
