cmake_minimum_required(VERSION 3.5)

project(QtGameFramework VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Multimedia REQUIRED)

set(PROJECT_SOURCES
       main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

        constant_set.h
        Framework/gamescene.h Framework/gamescene.cpp
        Framework/gameobject.h Framework/gameobject.cpp
        Framework/component.h Framework/component.cpp
        Framework/transform.h Framework/transform.cpp
        Framework/imagetransform.h Framework/imagetransform.cpp
        Framework/transformbuilder.h Framework/transformbuilder.cpp

        game_start.h game_start.cpp
        game_control.h game_control.cpp
        physics.h physics.cpp
        hitable.h hitable.cpp
        health.h health.cpp
        wall.h wall.cpp
        props.h props.cpp
        bomb.h bomb.cpp
        move_control.h move_control.cpp
        bomb_control.h bomb_control.cpp
        gamemap.h gamemap.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QtGameFramework
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QtGameFramework APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QtGameFramework SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QtGameFramework
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(QtGameFramework PRIVATE framework/)
target_link_libraries(QtGameFramework PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(QtGameFramework PRIVATE Qt6::Multimedia)

set_target_properties(QtGameFramework PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QtGameFramework)
endif()
