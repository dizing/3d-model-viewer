cmake_minimum_required(VERSION 3.5)

project(s21_viewer LANGUAGES C CXX VERSION 0.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options("-Wall" "-Wextra" "-g")
else()
    message("WE NEED GCC FOR CODE COVERAGE AND COMPILE OPTIONS REQUIRED BY SCHOOL")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/modules)

# FOR SCHOOL MAC'S HOMEBREW
list(APPEND CMAKE_SYSTEM_PREFIX_PATH /opt/goinfre/$ENV{USER}/homebrew/bin)
list(APPEND CMAKE_SYSTEM_PREFIX_PATH /opt/goinfre/$ENV{USER}/homebrew/sbin)

#PASS SOME PATHES TO CODE
#SETTINGS
cmake_path(APPEND SETTINGS_PATH "${CMAKE_BINARY_DIR}" "settings.conf")
#OBJ FILES FOR TESTS (NOT BE AVAILABLE AFTER SOURCE DELETING)
cmake_path(APPEND OBJECTS_PATH "${CMAKE_SOURCE_DIR}" "viewer_graph_lib" "test")
configure_file(settings_path.h.in settings_path.h @ONLY)

# MY MATH LIB WRITED IN C
add_subdirectory(viewer_graph_lib)



# FORMATTING
set(CLANG_FORMAT_EXCLUDE_PATTERNS  "build/" "dependencies/" ${CMAKE_BINARY_DIR})
find_package(ClangFormat)

# GENERATE DOCUMENTION
find_package(Doxygen)
if (Doxygen_FOUND)
    set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
    set(DOXYGEN_EXTRACT_STATIC YES)
    doxygen_add_docs(generate_docs)
endif()

# CPACK FOR DIST STANDARD GNU TARGET
SET(CPACK_SOURCE_GENERATOR "TGZ")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}")
SET(CPACK_SOURCE_IGNORE_FILES 
    "/build/;/buildDebug/;/buildRelease/;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        viewer_widget.h
        viewer_widget.cpp
        settings_widget.h
        settings_widget.cpp
        settings_widget.ui
)

#QT GUI APP
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(QT NAMES Qt6 Qt5 REQUIRED)

# DEPENDENCIES
add_subdirectory(dependencies)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets OpenGL)
    qt_add_executable(s21_viewer MANUAL_FINALIZATION ${PROJECT_SOURCES})
    target_link_libraries(s21_viewer PRIVATE viewer_graph_lib Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGLWidgets Qt${QT_VERSION_MAJOR}::OpenGL giflib)
else()
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Gui)
    add_executable(s21_viewer ${PROJECT_SOURCES})
    target_link_libraries(s21_viewer PRIVATE viewer_graph_lib Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Gui giflib)
endif()
target_include_directories(s21_viewer PUBLIC ${PROJECT_SOURCE_DIR}/dependencies/giflib ${PROJECT_SOURCE_DIR}/dependencies/gifimage)


set_target_properties(s21_viewer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(s21_viewer)
endif()
