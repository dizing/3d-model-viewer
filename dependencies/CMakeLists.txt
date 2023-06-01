file(GLOB dependencies_sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/*/*.c ${CMAKE_CURRENT_LIST_DIR}/*/*.cpp ${CMAKE_CURRENT_LIST_DIR}/*/*.h)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets OpenGL)
    add_library(giflib ${dependencies_sources})
    target_link_libraries(giflib PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGLWidgets Qt${QT_VERSION_MAJOR}::OpenGL)
else()
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Gui)
    add_library(giflib ${dependencies_sources})
    target_link_libraries(giflib PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Gui)
endif()
target_include_directories(giflib PUBLIC {CMAKE_CURRENT_LIST_DIR}/giflib {CMAKE_CURRENT_LIST_DIR}/gifimage)
target_compile_options(giflib PRIVATE "-w")
