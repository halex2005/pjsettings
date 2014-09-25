CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

include_directories(.)

add_executable(test-pjsettings-contrib
    main.cpp
    pjsettings-jsoncpp.tests.cpp
    pjsettings-pugixml.tests.cpp
    SimpleClass.h
)
target_link_libraries(test-pjsettings-contrib pjsettings-pugixml)
if (UNIX)
    add_definitions(-std=c++11)
    add_definitions(-fpermissive)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PJSIP libpjproject REQUIRED)
    if (NOT PJSIP_FOUND)
        message(SEND_ERROR "PJSIP libraries not found")
        return()
    endif()
    target_link_libraries(test-pjsettings-contrib ${PJSIP_LIBRARIES})
endif()

add_custom_command(
    TARGET test-pjsettings-contrib PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/test-config-pugixml.xml $<TARGET_FILE_DIR:test-pjsettings-contrib>)

add_custom_command(
    TARGET test-pjsettings-contrib PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/test-config-jsoncpp.json $<TARGET_FILE_DIR:test-pjsettings-contrib>)