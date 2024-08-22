cmake_minimum_required(VERSION 2.8.12)

project(echoserver LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Sql WebSockets REQUIRED)

add_executable(${PROJECT_NAME} "main.cpp"
    "dataaccessobject.cpp"
    "dataaccessobject.h"
    "echoserver.cpp"
    "echoserver.h"
    )

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Sql Qt5::WebSockets )
