cmake_minimum_required(VERSION 3.27)
project(piklema_test)

find_package(Qt5 COMPONENTS Core Quick Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
set(GLOB QML_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/qml/*.qml")
qt5_add_resources(RESOURCES ${QML_FILES})

message(TRACE ${RESOURCES})

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Quick Mqtt)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} resources.qrc)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick Qt5::Widgets Qt5::Mqtt)
