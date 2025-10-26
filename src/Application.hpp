#pragma once

#include "Simulator.hpp"
#include "Math.hpp"

#include <Eigen/Dense>

struct InputParameters {
    Eigen::Vector3f pSource;
    Eigen::Vector3f pTarget;
    float bSpeed;
    float bMass;

    float step;
};

/* CLI App */
class Application {

public:
    Application();

    void run();

private:
    void printHelp();

    void commandStart();
    void commandSetParam(const std::string_view line);

    float parseNumber(const std::string_view value, bool& parseError);
    Eigen::Vector3f parsePoint3(const std::string_view value, bool& parseError);

private:
    bool running;
    Simulator simulator;

    InputParameters input;
    bool canHit;
    float hitAngle;

};
