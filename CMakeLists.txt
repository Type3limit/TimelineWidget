cmake_minimum_required(VERSION 3.24)
project(TimelineWidget)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(
        ${PROJECT_SOURCE_DIR}/forms
        ${PROJECT_SOURCE_DIR}/headers
        ${PROJECT_SOURCE_DIR}/sources
        ${PROJECT_SOURCE_DIR}/extensions
)
find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(TimelineWidget main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        sources/timelinewidget.cpp
        headers/timelinewidget.h
        forms/timelinewidget.ui
        sources/tick/tickdrawingview.cpp
        headers/tick/tickdrawingview.h
        extensions/extensionmethods.h
        sources/ruler/rulerdrawingview.cpp
        headers/ruler/rulerdrawingview.h
        sources/trackbody/trackbodydrawingview.cpp
        headers/trackbody/trackbodydrawingview.h
        sources/trackhead/trackheaddrawingview.cpp
        headers/trackhead/trackheaddrawingview.h
        headers/timelinedefination.h
        headers/selfcontainedscenesview.h
        sources/tick/tickitem.cpp
        headers/tick/tickitem.h
        sources/ruler/ruleritem.cpp
        headers/ruler/ruleritem.h
        sources/ruler/anchorheaditem.cpp
        headers/ruler/anchorheaditem.h
        sources/trackbody/anchorbodyitem.cpp
        headers/trackbody/anchorbodyitem.h
        sources/trackhead/trackheaditem.cpp
        headers/trackhead/trackheaditem.h
        sources/trackbody/trackbodyitem.cpp
        headers/trackbody/trackbodyitem.h
        headers/mimedata/trackmime.h
        extensions/detail/itoa.h
        extensions/detail/stringstream.h
        extensions/detail/traits.h
        extensions/json.h
        extensions/reflection.h
        extensions/xml.h
        extensions/jsonserialization.h
        extensions/xmlserialization.h
        extensions/jsonserialization.h
        extensions/xmlserialization.h
        extensions/intervalwatcher.h
        headers/mimedata/clipmime.h
        sources/clip/clipitem.cpp
        headers/clip/clipitem.h
        headers/mimedata/timelinemime.h
        sources/clip/shadowclipitem.cpp
        headers/clip/shadowclipitem.h
        sources/trackbody/trackselectionitem.cpp
        headers/trackbody/trackselectionitem.h
        sources/mimedata/cliprange.cpp
        headers/mimedata/cliprange.h sources/clip/clipdraghandle.cpp headers/clip/clipdraghandle.h headers/clip/clipdraghandle.h headers/clip/clipdraghandle.h)


target_link_libraries(TimelineWidget
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

if (WIN32)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
