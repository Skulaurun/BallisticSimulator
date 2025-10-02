#pragma once

#include "Math.hpp"

#include <entt/entt.hpp>

struct Position {
    math::Point3f p;
};

struct Velocity {
    math::Point3f d;
};

struct RigidBody {
    float mass;
};

class Simulator {

public:
    Simulator();

    /*
    * source [m]
    * target [m]
    * 
    * speed [m/s]
    * mass [g]
    */
    void setParams(const math::Point3f source, const math::Point3f target, const float speed, const float mass);

    void run(const float step);

    /*
        return angle [deg]
    */
    float getHitAngle();

private:
    entt::entity spawnBullet(const float angle);

    void updatePhysics(const float step);
    void updateMovement(const float step);

private:
    entt::registry registry;

    // TODO: Make struct
    math::Point3f pSource;
    math::Point3f pTarget;
    float bSpeed;
    float bMass;

    float hitAngle;

};
