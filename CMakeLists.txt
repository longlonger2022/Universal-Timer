cmake_minimum_required(VERSION 3.16)
project(UniversalTimer2 LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
)
qt_standard_project_setup()

# Source files
set(PROJECT_SOURCES
    Universal-Timer-2/main.cpp
    Universal-Timer-2/UniversalTimer2.cpp
    Universal-Timer-2/UniversalTimer2.h
    Universal-Timer-2/UniversalTimer2.ui
    Universal-Timer-2/UniversalTimer2.qrc
    Universal-Timer-2/Resource.rc
)

# Qt resources
qt_add_resources(QT_RESOURCES
    Universal-Timer-2/UniversalTimer2.qrc
)

# Translation files
set(PROJECT_TRANSLATIONS
    Universal-Timer-2/lang/translation/translation_en.ts
    Universal-Timer-2/lang/translation/translation_zh-Classic-Hans.ts
    Universal-Timer-2/lang/translation/translation_zh-Classic-Hant.ts
)

# Windows icon
set(PROJECT_ICON
    Universal-Timer-2/Universal-Timer-2_icon.ico
)

qt_add_executable(${PROJECT_NAME} 
    ${PROJECT_SOURCES}
    ${QT_RESOURCES}
)

# Add WIN32_EXECUTABLE property for Windows
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Multimedia
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Universal-Timer-2/sound"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/sound"
    COMMENT "Copying sound files to output directory"
)