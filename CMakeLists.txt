cmake_minimum_required(VERSION 3.5)
project(ScribbleExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(ScribbleExample
    main.cpp
    mainwindow.cpp
    mainwindow.h
    paintview.cpp
    paintview.h
    tool.cpp
    tool.h
    penciltool.cpp
    penciltool.h
    hatchingtool.cpp
    hatchingtool.h
)

target_link_libraries(ScribbleExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
