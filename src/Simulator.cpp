#include "Simulator.hpp"

#include <chrono>
#include <thread>

#include <print>

Simulator::Simulator() :
    pSource{},
    pTarget{},
    bSpeed(0.0f),
    bMass(0.0f),
    hitAngle(0.0f)
{}

void Simulator::setParams(const math::Point3f source, const math::Point3f target, const float speed, const float mass) {
    pSource = source;
    pTarget = target;
    bSpeed = speed;
    bMass = mass;
}

void Simulator::run(const float step) {
    entt::entity target = registry.create();
    registry.emplace<Position>(target, Position { .p = pTarget });
    
    float angleGuess = 3.2f;
    entt::entity bullet = spawnBullet(angleGuess, target);
    RangeCollider& collider = registry.get<RangeCollider>(bullet);

    while (true) {
        updatePhysics(step);
        updateMovement(step);
        updateCollision();

        math::Point3f& t = registry.get<Position>(target).p;
        math::Point3f& b = registry.get<Position>(bullet).p;
        std::println("[{}, {}, {}], [{}, {}, {}]", t.x, t.y, t.z, b.x, b.y, b.z);

        if (collider.isHit || collider.isGroundHit) {
            break;
        }
    }

    hitAngle = angleGuess;
}

entt::entity Simulator::spawnBullet(const float angle, const entt::entity target) {
    float magnitude = std::sqrtf(pTarget.x * pTarget.x + pTarget.z * pTarget.z);
    float dx = bSpeed * (pTarget.x / magnitude) * std::cosf(math::radf(angle));
    float dz = bSpeed * (pTarget.z / magnitude) * std::cosf(math::radf(angle));
    float dy = bSpeed * std::sinf(math::radf(angle));

    entt::entity bullet = registry.create();
    registry.emplace<Position>(bullet, Position{ .p = pSource });
    registry.emplace<Velocity>(bullet, Velocity{ .d = { dx, dy, dz } });
    registry.emplace<RigidBody>(bullet, RigidBody{ .mass = bMass });
    registry.emplace<RangeCollider>(bullet, RangeCollider{ .target = target, .error = 10.0f });

    return bullet;
}

void Simulator::updatePhysics(const float step) {
    auto view = registry.view<Velocity, RigidBody>();
    for (auto [entity, velocity, body] : view.each()) {
        velocity.d.y -= 9.8 * step;
    }
}

void Simulator::updateMovement(const float step) {
    auto view = registry.view<Position, Velocity>();
    for (auto [entity, position, velocity] : view.each()) {
        position.p.x += velocity.d.x * step;
        position.p.y += velocity.d.y * step;
        position.p.z += velocity.d.z * step;
    }
}

void Simulator::updateCollision() {
    auto view = registry.view<Position, RangeCollider>();
    for (auto [entity, position, collider] : view.each()) {
        math::Point3f& a = position.p;
        math::Point3f& b = registry.get<Position>(collider.target).p;
        float distance = std::sqrtf(
            std::powf(b.x - a.x, 2) +
            std::powf(b.y - a.y, 2) +
            std::powf(b.z - a.z, 2)
        );
        if (distance <= collider.error) {
            collider.isHit = true;
        }
        if (a.y <= 0.0f) {
            collider.isGroundHit = true;
        }
    }
}

float Simulator::getHitAngle() const {
    return hitAngle;
}
