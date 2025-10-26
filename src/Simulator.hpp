#pragma once

#include "Math.hpp"

#include <entt/entt.hpp>
#include <Eigen/Dense>

struct Position {
    Eigen::Vector3f p;
};

struct Velocity {
    Eigen::Vector3f d;
};

struct RigidBody {
    float mass;
};

struct RangeCollider {
    entt::entity target;
    float error;

    bool isHit = false;
    
    // For simplicity
    bool isGroundHit = false;
};

class Simulator {

public:
    Simulator(const bool withDrag = true);

    /*
    * source [m,m,m]
    * target [m,m,m]
    * 
    * speed [m/s]
    * mass [kg]
    */
    void setParams(const Eigen::Vector3f source, const Eigen::Vector3f target, const float speed, const float mass);

    bool run(const float step);

    /*
        return angle [deg]
    */
    float getHitAngle() const;
    const std::vector<Eigen::Vector3f>& getBulletPath() const;

    void setDrag(const bool withDrag) { this->withDrag = withDrag; }

private:
    entt::entity spawnBullet(const float angle, const entt::entity target);

    void updatePhysics(const float step);
    void updateMovement(const float step);
    void updateCollision();

private:
    entt::registry registry;
    bool withDrag;

    // TODO: Make struct
    Eigen::Vector3f pSource;
    Eigen::Vector3f pTarget;
    float bSpeed;
    float bMass;

    float hitAngle;
    std::vector<Eigen::Vector3f> bulletPath;

};
