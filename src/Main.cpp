#include "Simulator.hpp"

#include <print>

int main() {
    
    Simulator simulator;

    simulator.setParams({ 0.0f, 10.0f, 0.0f }, { 10.0f, 1.0f, 10.0f }, 200.0f, 6.5f);
    bool canHit = simulator.run(0.01f);

    if (canHit) {
        std::println("Hit angle = {}deg", simulator.getHitAngle());
    } else {
        std::println("Solution not found or target is out of range!");
    }

    return 0;
}
