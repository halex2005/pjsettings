CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

include_directories(.)

add_executable(test-pjsettings
    main.cpp
    pjsettings-jsoncpp.tests.cpp
    pjsettings-pugixml.tests.cpp
    SimpleClass.h
    test-config-jsoncpp.json
    test-config-pugixml.xml
)
target_link_libraries(test-pjsettings pjsettings)
if (UNIX)
    add_definitions(-std=c++11)
    add_definitions(-fpermissive)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PJSIP libpjproject REQUIRED)
    if (NOT PJSIP_FOUND)
        message(SEND_ERROR "PJSIP libraries not found")
        return()
    endif()
    target_link_libraries(test-pjsettings ${PJSIP_LIBRARIES})
endif()

add_custom_command(
    TARGET test-pjsettings PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/test-config-pugixml.xml $<TARGET_FILE_DIR:test-pjsettings>)

add_custom_command(
    TARGET test-pjsettings PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/test-config-jsoncpp.json $<TARGET_FILE_DIR:test-pjsettings>)
