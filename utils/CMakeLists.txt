set(NAME utils)

set(SOURCES
    chartFormatter.cpp
)

add_library(${NAME} STATIC ${SOURCES})

target_link_libraries(${NAME}
    Qt${QT_VERSION_MAJOR}::Charts
)

target_include_directories(${NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc/${NAME})
