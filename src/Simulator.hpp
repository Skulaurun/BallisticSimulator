#pragma once

#include <entt/entt.hpp>

struct Point3D {
    float x;
    float y;
    float z;
};

struct Position {
    Point3D p;
};

struct Velocity {
    Point3D d;
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
    void setParams(const Point3D source, const Point3D target, const float speed, const float mass);

    void run(const float step);

    /*
        return angle [deg]
    */
    float getHitAngle();

private:
    entt::registry registry;

    // TODO: Make struct
    Point3D pSource;
    Point3D pTarget;
    float bSpeed;
    float bMass;

    float hitAngle;

};
