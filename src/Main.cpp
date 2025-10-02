#include "Simulator.hpp"

#include <print>

int main() {
    
    Simulator simulator;

    simulator.setParams({ 0.0f, 1.0f, 0.0f }, { 400.0f, 1.0f, 80.0f }, 200.0f, 6.5f);
    simulator.run(0.25f);

    std::println("Hit angle = {}deg", simulator.getHitAngle());

    return 0;
}
