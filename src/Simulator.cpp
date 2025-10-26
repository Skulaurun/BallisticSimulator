#include "Simulator.hpp"

#include <chrono>
#include <thread>

#include <print>

Simulator::Simulator(const bool withDrag) :
    pSource{},
    pTarget{},
    bSpeed(0.0f),
    bMass(0.0f),
    hitAngle(0.0f),
    withDrag(withDrag)
{}

void Simulator::setParams(const Eigen::Vector3f source, const Eigen::Vector3f target, const float speed, const float mass) {
    pSource = source;
    pTarget = target;
    bSpeed = speed;
    bMass = mass;
}

bool Simulator::run(const float step) {
    entt::entity target = registry.create();
    registry.emplace<Position>(target, Position { .p = pTarget });
    
    // Probably would be better to spawn all projectiles at once, and then update
    // Keeping all paths in memory could be a problem,
    // so probably find angle first, and then run simulation again for the angle to build a path
    for (float angleGuess = 0.0f; angleGuess <= 90.0f; angleGuess += 0.1f) {
        entt::entity bullet = spawnBullet(angleGuess, target);
        RangeCollider& collider = registry.get<RangeCollider>(bullet);

        Eigen::Vector3f& position = registry.get<Position>(bullet).p;
        bulletPath.clear();

        while (true) {
            updatePhysics(step);
            updateMovement(step);
            updateCollision();

            bulletPath.push_back(position);

            if (collider.isHit) {
                registry.destroy(bullet);
                hitAngle = angleGuess;
                return true;
            }

            if (collider.isGroundHit) {
                registry.destroy(bullet);
                break;
            }
        }
    }

    registry.destroy(target);
    return false;
}

entt::entity Simulator::spawnBullet(const float angle, const entt::entity target) {
    Eigen::Vector3f direction = pTarget - pSource;
    direction.y() = 0;
    direction.normalize();

    Eigen::Vector3f velocity = {
        bSpeed * direction.x() * std::cosf(math::radf(angle)),
        bSpeed * std::sinf(math::radf(angle)),
        bSpeed * direction.z() * std::cosf(math::radf(angle))
    };

    entt::entity bullet = registry.create();
    registry.emplace<Position>(bullet, Position{ .p = pSource });
    registry.emplace<Velocity>(bullet, Velocity{ .d = velocity });
    registry.emplace<RigidBody>(bullet, RigidBody{ .mass = bMass });
    registry.emplace<RangeCollider>(bullet, RangeCollider{ .target = target, .error = 1.0f });

    return bullet;
}

void Simulator::updatePhysics(const float step) {
    auto view = registry.view<Velocity, RigidBody>();
    for (auto [entity, velocity, body] : view.each()) {
        Eigen::Vector3f& v = velocity.d;

        /*
            Source: https://www.youtube.com/watch?v=iwfeqRBm3LQ
        */

        if (withDrag) {
            constexpr float rho = 1.2f; // [kg/m^3]
            constexpr float R = 0.155f; // 155mm
            constexpr float A = float(M_PI) * R * R;
            constexpr float Cd = 0.295f; // Drag coefficient
        
            float vLength = v.norm();
            Eigen::Vector3f vUnit = v.normalized();
            Eigen::Vector3f F = -vUnit * 0.5f * A * Cd * vLength * vLength;
        
            // Apply Quadratic Drag Force
            v += (F / body.mass) * step;
        }

        // Apply Gravity
        v.y() += -9.8f * step;
    }
}

void Simulator::updateMovement(const float step) {
    auto view = registry.view<Position, Velocity>();
    for (auto [entity, position, velocity] : view.each()) {
        position.p.x() += velocity.d.x() * step;
        position.p.y() += velocity.d.y() * step;
        position.p.z() += velocity.d.z() * step;
    }
}

void Simulator::updateCollision() {
    auto view = registry.view<Position, RangeCollider>();
    for (auto [entity, position, collider] : view.each()) {
        Eigen::Vector3f& a = position.p;
        Eigen::Vector3f& b = registry.get<Position>(collider.target).p;
        float distance = (b - a).squaredNorm();
        if (distance <= collider.error * collider.error) {
            collider.isHit = true;
        }
        if (a.y() <= 0.0f) {
            collider.isGroundHit = true;
        }
    }
}

float Simulator::getHitAngle() const {
    return hitAngle;
}

const std::vector<Eigen::Vector3f>& Simulator::getBulletPath() const {
    return bulletPath;
}
