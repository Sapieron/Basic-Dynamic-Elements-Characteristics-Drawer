set(NAME calc_amp_phase_test)

add_executable(${NAME}
    amp_phase_tests.cpp
)
target_link_libraries(${NAME}
    calc_amp_phase
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(${NAME})
