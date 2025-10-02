add_executable(ShootingSimulator
    "Main.cpp"
    "Simulator.cpp"
)

target_link_libraries(ShootingSimulator
    EnTT::EnTT
)
