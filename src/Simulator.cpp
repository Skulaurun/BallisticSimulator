#include "Simulator.hpp"

#define _USE_MATH_DEFINES

#include <chrono>
#include <thread>
#include <cmath>
#include <print>

Simulator::Simulator() :
    pSource{},
    pTarget{},
    bSpeed(0.0f),
    bMass(0.0f),
    hitAngle(0.0f)
{}

void Simulator::setParams(const Point3D source, const Point3D target, const float speed, const float mass) {
    pSource = source;
    pTarget = target;
    bSpeed = speed;
    bMass = mass;
}

void Simulator::run(const float step) {

    entt::entity target = registry.create();
    registry.emplace<Position>(target, Position { .p = pTarget });

    float angleGuess = 3.15f;
    float magnitude = std::sqrtf(pTarget.x * pTarget.x + pTarget.z * pTarget.z);
    float dx = bSpeed * (pTarget.x / magnitude);
    float dz = bSpeed * (pTarget.z / magnitude);
    float dy = bSpeed * std::sinf(angleGuess * (M_PI / 180.0f));

    entt::entity bullet = registry.create();
    registry.emplace<Position>(bullet, Position{ .p = pSource });
    registry.emplace<Velocity>(bullet, Velocity{ .d = { dx, dy, dz } });
    registry.emplace<RigidBody>(bullet, RigidBody{ .mass = bMass });

    while (true) {
        for (auto [entity, velocity, body] : registry.view<Velocity, RigidBody>().each()) {
            velocity.d.y -= 9.8 * step;
        }

        for (auto [entity, position, velocity] : registry.view<Position, Velocity>().each()) {
            position.p.x += velocity.d.x * step;
            position.p.y += velocity.d.y * step;
            position.p.z += velocity.d.z * step;
        }

        Point3D& t = registry.get<Position>(target).p;
        Point3D& b = registry.get<Position>(bullet).p;
        std::println("[{}, {}, {}], [{}, {}, {}]", t.x, t.y, t.z, b.x, b.y, b.z);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (b.y <= 0.0f) {
            /* Hit ground */
            break;
        }
    }

}

float Simulator::getHitAngle() {

    return 0.0f;
}
