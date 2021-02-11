set(NAME widget)

set(SOURCES
    themewidget.cpp
    themewidget.ui
    inc/${NAME}/themewidget.h
)

add_library(${NAME} STATIC ${SOURCES})

target_link_libraries(${NAME}
    calculation
    utils
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Charts
)

target_include_directories(${NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc/${NAME})
