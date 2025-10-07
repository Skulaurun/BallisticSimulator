#include "Math.hpp"
#include "Simulator.hpp"

#include <catch2/catch_all.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

constexpr float g = 9.8;

static float vacMaxRange(const float v0) {
    /*
        Source: The maximum range for projectile motion with quadratic drag is not an analytic formula (but kinda close) - John Bradshaw
    */
    return (v0 * v0) / g * sin(2 * math::radf(45));
}

static float xzDistance(const math::Point3f& A, const math::Point3f& B) {
    float a = B.x - A.x;
    float b = B.z - A.z;
    return std::sqrtf(a * a + b * b);
}

TEST_CASE("target is out of range (no drag)") {
    const math::Point3f source = { 0.0f, 1.0f, 0.0f };
    const math::Point3f target = { 600.0f, 1.0f, 600.0f };
    const float v0 = 40.0f;

    REQUIRE(xzDistance(source, target) > vacMaxRange(v0));

    Simulator simulator(false);
    simulator.setParams(source, target, v0, 20.0f);
    bool canHit = simulator.run(0.01f);
    REQUIRE(!canHit);
}

TEST_CASE("target is hit (no drag)") {
    const math::Point3f source = { 0.0f, 1.0f, 0.0f };
    const math::Point3f target = { 100.0f, 1.0f, 100.0f };
    const float v0 = 40.0f;

    REQUIRE(xzDistance(source, target) < vacMaxRange(v0));

    Simulator simulator(false);
    simulator.setParams(source, target, v0, 20.0f);
    bool canHit = simulator.run(0.01f);
    REQUIRE(canHit);

    const math::Point3f& last = simulator.getBulletPath().back();
    REQUIRE_THAT(target.x, Catch::Matchers::WithinAbs(last.x, 1.0f));
    REQUIRE_THAT(target.y, Catch::Matchers::WithinAbs(last.y, 1.0f));
    REQUIRE_THAT(target.z, Catch::Matchers::WithinAbs(last.z, 1.0f));
}

TEST_CASE("elevation angle (no drag)") {
    const math::Point3f source = { 0.0f, 41.0f, 0.0f };
    const math::Point3f target = { 600.0f, 1.0f, 600.0f };
    const float v0 = 100.0f;

    float h = std::abs(target.y - source.y);
    float x = xzDistance(source, target);

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

    Simulator simulator(false);
    simulator.setParams(source, target, v0, 6.5f);
    bool canHit = simulator.run(0.01f);
    REQUIRE(canHit);
    
    float angle = simulator.getHitAngle();
    REQUIRE_THAT(angle, Catch::Matchers::WithinAbs(theta1, 1.0f)
        || Catch::Matchers::WithinAbs(theta2, 1.0f));
}

TEST_CASE("elevation angle (with drag)") {
    const math::Point3f source = { 0.0f, 41.0f, 0.0f };
    const math::Point3f target = { 600.0f, 1.0f, 600.0f };
    const float v0 = 100.0f;
    const float dt = 0.01f;

    Simulator simulator(true);
    simulator.setParams(source, target, v0, 40.0f);
    bool canHit = simulator.run(dt);
    REQUIRE(canHit);
    /* With drag */
    float theta1 = simulator.getHitAngle();

    simulator.setDrag(false);
    canHit = simulator.run(dt);
    REQUIRE(canHit);
    /* Without drag */
    float theta2 = simulator.getHitAngle();

    REQUIRE(theta1 > theta2);

    /*
        The heavier the projectile, the lesser is the deceleration caused by drag force,
        with heavy enough projectile the result should be close to that with no drag
    */
    simulator.setDrag(true);
    simulator.setParams(source, target, v0, 9999.0f);
    canHit = simulator.run(dt);
    REQUIRE(canHit);
    float theta3 = simulator.getHitAngle();

    REQUIRE_THAT(theta3, Catch::Matchers::WithinAbs(theta2, 1.0f));
}
