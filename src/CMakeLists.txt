﻿cmake_minimum_required(VERSION 3.5)

project(StudentManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Charts Sql WebSockets Xml SerialPort PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts Sql WebSockets Xml SerialPort PrintSupport)

include_directories(${LIBSSH_INCLUDE_DIRS})

file(GLOB_RECURSE CPP_FILES  *.cpp *.h)
foreach(filepath ${CPP_FILES})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND example_files ${filename})
endforeach(filepath)

include_directories(.)

set(PROJECT_SOURCES
        ${example_files}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(StudentManager
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

    #遍历所有资源文件
    file(GLOB_RECURSE RES_PATHS *.png *.jpg *.svg *.ico *.ttf *.webp *.js)
    foreach(filepath ${RES_PATHS})
            string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
            list(APPEND resource_files ${filename})
    endforeach(filepath)

    qt_add_resources(${PROJECT_NAME} "StudentManager"
        RESOURCES PREFIX "/"
        FILES
        ${resource_files}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET StudentManager APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(StudentManager SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(StudentManager
            ${PROJECT_SOURCES}
            ../src/include/ElaWidgetTools.qrc
            StudentManager.qrc
        )
    endif()
endif()

include_directories(
    ../libs/include
    Page
    ModelView
)
set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR})
target_link_libraries(StudentManager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts
Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::WebSockets Qt${QT_VERSION_MAJOR}::Xml Qt${QT_VERSION_MAJOR}::SerialPort
Qt${QT_VERSION_MAJOR}::PrintSupport
    elawidgettools  ssh
)
# 包含 OpenCV 头文件
target_include_directories(StudentManager PRIVATE ${OpenCV_INCLUDE_DIRS})

# 链接 OpenCV 库
target_link_libraries(StudentManager PRIVATE ${OpenCV_LIBS})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.aacore.desktopExample)
endif()
set_target_properties(StudentManager PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(StudentManager PUBLIC
    sqlite3)

include(GNUInstallDirs)
install(TARGETS StudentManager
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(StudentManager)
endif()
