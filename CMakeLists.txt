cmake_minimum_required(VERSION 3.24)
project(TimelineWidget)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(
        ${PROJECT_SOURCE_DIR}/Forms
        ${PROJECT_SOURCE_DIR}/Headers
        ${PROJECT_SOURCE_DIR}/Sources
        ${PROJECT_SOURCE_DIR}/Extensions
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
        Sources/timelinewidget.cpp
        Headers/timelinewidget.h
        Forms/timelinewidget.ui
        Sources/Tick/tickdrawingview.cpp
        Headers/Tick/tickdrawingview.h
        Extensions/extensionmethods.h
        Sources/Ruler/rulerdrawingview.cpp
        Headers/Ruler/rulerdrawingview.h
        Sources/TrackBody/trackbodydrawingview.cpp
        Headers/TrackBody/trackbodydrawingview.h
        Sources/TrackHead/trackheaddrawingview.cpp
        Headers/TrackHead/trackheaddrawingview.h
        Headers/timelinedefination.h
        Headers/selfcontainedscenesview.h
        Sources/Tick/tickitem.cpp
        Headers/Tick/tickitem.h
        Sources/Ruler/ruleritem.cpp
        Headers/Ruler/ruleritem.h
        Sources/Ruler/anchorheaditem.cpp
        Headers/Ruler/anchorheaditem.h
        Sources/TrackBody/anchorbodyitem.cpp
        Headers/TrackBody/anchorbodyitem.h
        Sources/TrackHead/trackheaditem.cpp
        Headers/TrackHead/trackheaditem.h
        Sources/TrackBody/trackbodyitem.cpp
        Headers/TrackBody/trackbodyitem.h
        Headers/MimeData/trackmime.h
        Extensions/detail/itoa.h
        Extensions/detail/stringstream.h
        Extensions/detail/traits.h
        Extensions/json.h
        Extensions/reflection.h
        Extensions/xml.h
        Extensions/jsonserialization.h
        Extensions/xmlserialization.h
        Extensions/jsonserialization.h
        Extensions/xmlserialization.h
        Extensions/intervalwatcher.h
        Headers/MimeData/clipmime.h
        Sources/Clip/clipitem.cpp
        Headers/Clip/clipitem.h
        Headers/MimeData/timelinemime.h
        Sources/Clip/shadowclipitem.cpp
        Headers/Clip/shadowclipitem.h
        Sources/TrackBody/trackselectionitem.cpp
        Headers/TrackBody/trackselectionitem.h
        Sources/MimeData/cliprange.cpp
        Headers/MimeData/cliprange.h Sources/Clip/clipdraghandle.cpp Headers/Clip/clipdraghandle.h Headers/Clip/clipdraghandle.h Headers/Clip/clipdraghandle.h)


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
