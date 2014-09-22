CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

include_directories(.)

add_executable(test-pjsettings-contrib
    main.cpp
    pjsettings-jsoncpp.tests.cpp
    pjsettings-pugixml.tests.cpp
)