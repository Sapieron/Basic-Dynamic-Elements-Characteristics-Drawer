set(NAME calc_amp_phase)

set(SOURCES
    amp_phase.cpp
)

add_library(${NAME} STATIC ${SOURCES})

target_link_libraries(${NAME}
    calculations
)

target_include_directories(${NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_include_directories(${NAME} PUBLIC    ${CMAKE_CURRENT_SOURCE_DIR}/inc/calculation/amp_phase)