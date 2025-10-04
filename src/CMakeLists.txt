set(SIMULATOR_SOURCES
    "Simulator.cpp"
    CACHE INTERNAL ""
)

add_executable(ShootingSimulator
    "Main.cpp"
    ${SIMULATOR_SOURCES}
)

target_link_libraries(ShootingSimulator
    EnTT::EnTT
)
