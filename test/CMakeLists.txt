set(SIMULATOR_SOURCE_DIR "../src")
list(TRANSFORM SIMULATOR_SOURCES PREPEND "${SIMULATOR_SOURCE_DIR}/" OUTPUT_VARIABLE SIMULATOR_SOURCES_TESTS)

add_executable(Test
    ${SIMULATOR_SOURCES_TESTS}
    "Simulator_test.cpp"
)

target_include_directories(Test PUBLIC
    ${SIMULATOR_SOURCE_DIR}
)

target_link_libraries(Test
    EnTT::EnTT
    Eigen3::Eigen
    Catch2::Catch2WithMain
)
