cmake_minimum_required(VERSION 3.16)

project(elgato_lights_manager_linux LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Widgets)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

qt_add_executable(${PROJECT_NAME}
    resources.qrc
    main.cpp
    lights.cpp
    light.cpp
    light.h
    lights.h
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt6::Core Qt6::Gui Qt6::Quick Qt6::Widgets
)

find_library(AVAHI_CLIENT_LIB avahi-client REQUIRED)
find_library(AVAHI_COMMON_LIB avahi-common REQUIRED)

find_path(AVAHI_INCLUDE_DIR avahi-client/client.h REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${AVAHI_CLIENT_LIB}
        ${AVAHI_COMMON_LIB}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${AVAHI_INCLUDE_DIR}
)
