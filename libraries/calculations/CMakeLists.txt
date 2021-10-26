set(NAME calculations)

set(SOURCES
    ICalculator.cpp
    # calculations.cpp //TODO add this file
)

add_subdirectory(amp_phase)
add_subdirectory(pid)
add_subdirectory(value_time)

add_library(${NAME} STATIC ${SOURCES})

target_link_libraries(${NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Charts
)

target_include_directories(${NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc/${NAME})
