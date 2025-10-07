#include "Math.hpp"
#include "Simulator.hpp"

#include <catch2/catch_all.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("elevation angle (no drag)") {
    Simulator simulator(false);

    const math::Point3f source = { 0.0f, 41.0f, 0.0f };
    const math::Point3f target = { 600.0f, 1.0f, 600.0f };
    const float v0 = 100.0f;
    const float g = 9.8;

    float a = target.x - source.x;
    float b = target.z - source.z;
    float h = std::abs(target.y - source.y);
    float x = std::sqrtf(a * a + b * b);

    /*
        Source: https://www.youtube.com/watch?v=bqYtNrhdDAY
    */
    //float phi = std::atanf(x / h);
    //float theta = (
    //    std::acosf(
    //        ((g * x * x) / (v0 * v0) - h) / std::sqrtf(h * h + x * x)
    //    ) + phi
    //) / 2.0f;
    //theta = math::degf(theta);

    /*
        Source: https://www.youtube.com/watch?v=32PiZDW40VI
    */
    float C = 0.5f * g * (x * x) / (v0 * v0);
    float D = x * x - 4.0f * (-C) * (-C + h);
    float X1 = (-x + std::sqrtf(D)) / (2.0f * -C);
    float X2 = (-x - std::sqrtf(D)) / (2.0f * -C);
    float theta1 = std::atanf(X1);
    float theta2 = std::atanf(X2);
    theta1 = math::degf(theta1);
    theta2 = math::degf(theta2);

    simulator.setParams(source, target, v0, 6.5f);
    bool canHit = simulator.run(0.01f);
    REQUIRE(canHit);
    
    float angle = simulator.getHitAngle();
    REQUIRE_THAT(angle, Catch::Matchers::WithinAbs(theta1, 1.0f)
        || Catch::Matchers::WithinAbs(theta2, 1.0f));
}

TEST_CASE("out of range") {
    Simulator simulator;
    simulator.setParams({ 0.0f, 1.0f, 0.0f }, { 600.0f, 1.0f, 10.0f }, 10.0f, 6.5f);
    bool canHit = simulator.run(0.01f);
    REQUIRE(!canHit);
}
