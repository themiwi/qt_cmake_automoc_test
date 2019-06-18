cmake_minimum_required(VERSION 3.14)

project(qt_cmake_moc_test CXX)

option(CMAKE_AUTOMOC "Use the AUTOMOC feature" ON)

find_package(Qt5 COMPONENTS Core Widgets CONFIG REQUIRED)

################################################################################
# mywidgets library
################################################################################

add_library(mywidgets SHARED)
target_link_libraries(mywidgets Qt5::Core Qt5::Widgets)

if (NOT CMAKE_AUTOMOC)
    qt5_wrap_cpp(MOC_SRCS mainwindow.h)
endif ()

target_sources(mywidgets
    PRIVATE
    mainwindow.cpp
    ${MOC_SRCS}

    PUBLIC
    mainwindow.h
    )


################################################################################
# executable
################################################################################

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} mywidgets Qt5::WinMain)

################################################################################
# copy depended-on DLL's to build dir (skip install dir in this example)
################################################################################

if (WIN32)
    string(REPLACE "\\" "/" Qt5_DIR "${Qt5_DIR}")
    set(QT_PLUGINS_DIR "${Qt5_DIR}/../../../plugins")
    set(DLL_SUFFIX "$<$<CONFIG:Debug>:d>")
    set(required_qt_plugins
        "${QT_PLUGINS_DIR}/platforms/qwindows${DLL_SUFFIX}.dll"
        "${QT_PLUGINS_DIR}/styles/qwindowsvistastyle${DLL_SUFFIX}.dll"
        )

    set(LIBS_TO_FIXUP)
    foreach (plugin IN LISTS required_qt_plugins)
        get_filename_component(plugin_file "${plugin}" NAME)
        get_filename_component(plugin_dest "${plugin}" DIRECTORY)
        get_filename_component(plugin_dest "${plugin_dest}" NAME)

        # copy the plugin to the build dir
        set(plugin_dest "${SYSTEMATIC_BINARY_DIRECTORY}/${plugin_dest}/${plugin_file}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${plugin}" "${plugin_dest}"
            VERBATIM)
        list(APPEND LIBS_TO_FIXUP "${plugin_dest}")
    endforeach ()

    # fixup up for the build dir
    set(EXE_TO_FIXUP "$<TARGET_FILE:${PROJECT_NAME}>")
    # first configure the template...
    configure_file(copy_libraries.cmake.in copy_libraries_build.cmake.in @ONLY)
    # ... then use file(GENERATE) to expand the generator expressions
    file(GENERATE
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/copy_libraries_build.cmake"
        INPUT "${CMAKE_CURRENT_BINARY_DIR}/copy_libraries_build.cmake.in")
    # execute the script
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -P copy_libraries_build.cmake
        VERBATIM)
endif ()
