cmake_minimum_required(VERSION 3.16)

project(alpha_project VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB PROJECT_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

if(NOT PROJECT_HEADERS AND NOT PROJECT_SOURCES)
    message(FATAL_ERROR "No source files were found in include/ or src/")
endif()

add_library(alpha_project_lib STATIC
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
    ui/homescreen.ui
    ui/notes.ui
    ui/noteseditor.ui
    ui/settings.ui
)

target_include_directories(alpha_project_lib
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(alpha_project_lib
    PUBLIC
        Qt6::Widgets
)

if(APPLE)
    target_compile_definitions(alpha_project_lib PUBLIC PLATFORM_MACOS=1)
elseif(WIN32)
    target_compile_definitions(alpha_project_lib PUBLIC PLATFORM_WINDOWS=1)
elseif(UNIX)
    target_compile_definitions(alpha_project_lib PUBLIC PLATFORM_LINUX=1)
endif()

if(MSVC)
    target_compile_options(alpha_project_lib PRIVATE /W4)
else()
    target_compile_options(alpha_project_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

set(APP_MAIN
    "${CMAKE_CURRENT_SOURCE_DIR}/app/main.cpp"
)

if(EXISTS "${APP_MAIN}")
    add_executable(alpha_project_app "${APP_MAIN}")
    target_link_libraries(alpha_project_app PRIVATE alpha_project_lib)
    set_target_properties(alpha_project_app PROPERTIES OUTPUT_NAME "alpha_project")
else()
    message(STATUS "No app/main.cpp found. The library target 'alpha_project_lib' will be generated for now.")
endif()
