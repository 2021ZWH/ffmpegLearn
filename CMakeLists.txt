cmake_minimum_required(VERSION 3.5)

project(VideoDecode VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(/utf-8)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Multimedia)

set(CMAKE_MODULE_PATH C:\\zwh\\3rdparty\\.cmake)

find_package(ffmpeg REQUIRED)

message(STATUS ${FFMPEG_INCLUDE_DIRS})
message(STATUS ${FFMPEG_LIBS})
set(PROJECT_SOURCES
    main.cpp
)
include_directories(${FFMPEG_INCLUDE_DIRS})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(VideoDecode
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET VideoDecode APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(VideoDecode SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(VideoDecode
            ${PROJECT_SOURCES}
            main.cpp
            widget.cpp widget.h widget.ui
            playstate.h
            audioplay.cpp audioplay.h
            blockingqueue.cpp blockingqueue.h
            demuxthread.cpp demuxthread.h
            avpacketqueue.h
            decodethread.cpp decodethread.h
            avframequeue.h
            i420render.cpp i420render.h
            avsynctimer.h
            frameconvert.h frameconvert.cpp
            avprogressbar.h avprogressbar.cpp
            avprogressgroup.h avprogressgroup.cpp

        )
    endif()
endif()


target_link_libraries(VideoDecode PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(VideoDecode PRIVATE Qt5::Multimedia)
target_link_libraries(VideoDecode PRIVATE ${FFMPEG_LIBS})
#target_link_libraries(VideoDecode PRIVATE ${FFMPEG_BINS})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VideoDecode)
endif()
set_target_properties(VideoDecode PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS VideoDecode
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(VideoDecode)
endif()
