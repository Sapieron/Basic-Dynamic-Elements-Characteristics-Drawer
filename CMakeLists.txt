cmake_minimum_required(VERSION 3.8)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

project(Drawer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Charts LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Charts LinguistTools REQUIRED)

add_subdirectory(libraries)

add_subdirectory(calculation)
add_subdirectory(widget)
add_subdirectory(utils)

set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/Drawer_PL.ts
)

# generate qm files
qt5_add_translation(QM_FILES ${TS_FILES})

add_executable(${PROJECT_NAME}
    main.cpp
    ${QM_FILES}
)


target_link_libraries(Drawer
    PRIVATE
    calculation
    widget
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Charts
)


qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
