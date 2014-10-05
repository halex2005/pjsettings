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
target_link_libraries(test-pjsettings pjsettings ${Boost_LIBRARIES} ${PJSIP_STATIC_LIBRARIES})

add_custom_command(
    TARGET test-pjsettings PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/test-config-pugixml.xml $<TARGET_FILE_DIR:test-pjsettings>)

add_custom_command(
    TARGET test-pjsettings PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/test-config-jsoncpp.json $<TARGET_FILE_DIR:test-pjsettings>)
