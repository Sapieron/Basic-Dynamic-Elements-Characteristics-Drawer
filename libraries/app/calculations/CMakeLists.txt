set(NAME app_calculations)

set(SOURCES
    calculations.cpp
)

add_library(${NAME} STATIC ${SOURCES})

target_link_libraries(${NAME}
    calculations
    Qt${QT_VERSION_MAJOR}::Core
)

target_include_directories(${NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc/app/calculations)