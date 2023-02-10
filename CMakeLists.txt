cmake_minimum_required(VERSION 3.24)
project(TimelineWidget)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")

#set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64")

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
        Sources/Tick/TickDrawingView.cpp
        Headers/Tick/TickDrawingView.h
        Extensions/ExtensionMethods.h
        Sources/Ruler/RulerDrawingView.cpp
        Headers/Ruler/RulerDrawingView.h
        Sources/TrackBody/TrackBodyDrawingView.cpp
        Headers/TrackBody/TrackBodyDrawingView.h
        Sources/TrackHead/TrackHeadDrawingView.cpp
        Headers/TrackHead/TrackHeadDrawingView.h
        Headers/TimelineDefination.h
        Headers/SelfContainedScenesView.h
        Sources/Tick/TickItem.cpp
        Headers/Tick/TickItem.h
        Sources/Ruler/RulerItem.cpp
        Headers/Ruler/RulerItem.h
        Sources/Ruler/AnchorHeadItem.cpp
        Headers/Ruler/AnchorHeadItem.h
        Sources/TrackBody/AnchorBodyItem.cpp
        Headers/TrackBody/AnchorBodyItem.h
        Sources/TrackHead/TrackHeadItem.cpp
        Headers/TrackHead/TrackHeadItem.h
        Sources/TrackBody/TrackBodyItem.cpp
        Headers/TrackBody/TrackBodyItem.h
        Headers/MimeData/TrackMime.h
        Extensions/detail/itoa.h
        Extensions/detail/string_stream.h
        Extensions/detail/traits.h
        Extensions/json.h
        Extensions/reflection.h
        Extensions/xml.h
        Extensions/JsonSerialization.h
        Extensions/XmlSerialization.h
        Extensions/JsonSerialization.h
        Extensions/XmlSerialization.h
        Extensions/IntervalWatcher.h
        Sources/TimelineDataManager.cpp
        Headers/TimelineDataManager.h
        Headers/MimeData/ClipMime.h
        Sources/Clip/ClipItem.cpp
        Headers/Clip/ClipItem.h Headers/MimeData/TimelineMime.h)

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
