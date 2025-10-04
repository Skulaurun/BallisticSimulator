#include "Application.hpp"

#include <print>
#include <string>
#include <regex>
#include <charconv>
#include <iostream>

Application::Application() : running(false), canHit(false), hitAngle(0.0f) {
    input = InputParameters{
        .pSource = math::Point3f{ 0.0f, 0.0f, 0.0f },
        .pTarget = math::Point3f{ 600.0f, 10.0f, 400.0f },
        .bSpeed = 100.0f,
        .bMass = 6.5f,

        .step = 0.01f
    };
}

void Application::run() {
    running = true;

    printHelp();

    while (running) {
        std::string line;
        std::getline(std::cin, line);

        if (line == "run") {
            commandStart();
        } else if (line == "q") {
            running = false;
        } else {
            std::regex setParam(".+=.+");
            if (std::regex_match(line, setParam)) {
                commandSetParam(line);
            } else {
                printHelp();
            }
        }
    }
}

void Application::printHelp() {
    std::println("Input Params:");
    std::println("* source=({};{};{}) [m]", input.pSource.x, input.pSource.y, input.pSource.z);
    std::println("* target=({};{};{}) [m]", input.pTarget.x, input.pTarget.y, input.pTarget.z);
    std::println("* velocity={} [m/s]", input.bSpeed);
    std::println("* step={}", input.step);

    std::println("\nType 'param=value' to change input params");
    std::println("Type 'run' to start the simulation");
    std::println("Type 'q' to quit");
}

void Application::commandStart() {
    std::println("Running simulation...");

    simulator.setParams(input.pSource, input.pTarget, input.bSpeed, input.bMass);
    canHit = simulator.run(input.step);
    if (canHit) {
        hitAngle = simulator.getHitAngle();
        std::println("Hit at \x1B[92m{}deg\033[0m", hitAngle);
    } else {
        std::println("Solution not found, is target out of range?");
    }
}

void Application::commandSetParam(const std::string_view line) {

    std::size_t position = line.find('=');
    std::string_view key = line.substr(0, position);
    std::string_view value = line.substr(position + 1);

    bool parseError = false;
    if (key == "source") {
        math::Point3f result = parsePoint3(value, parseError);
        if (!parseError) {
            input.pSource = result;
        }
    } else if (key == "target") {
        math::Point3f result = parsePoint3(value, parseError);
        if (!parseError) {
            input.pTarget = result;
        }
    } else if (key == "velocity") {
        float result = parseNumber(value, parseError);
        if (!parseError) {
            input.bSpeed = result;
        }
    } else if (key == "step") {
        float result = parseNumber(value, parseError);
        if (!parseError) {
            input.step = result;
        }
    } else {
        std::println("Error: Unknown param!");
    }

    printHelp();

}

float Application::parseNumber(const std::string_view value, bool& parseError) {
    std::string s(value);
    float output = 0.0f;
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), output);
    if (ec != std::errc()) {
        parseError = true;
        std::println("Error: Not a number!");
    }
    return output;
}

math::Point3f Application::parsePoint3(const std::string_view value, bool& parseError) {
    math::Point3f output = {};
    std::string s(value);

    bool isOk = true;
    std::regex point("\(.+;.+;.+\)");
    if (std::regex_match(s, point)) {
        std::size_t p1 = s.find_first_of(';');
        std::size_t p2 = s.find_last_of(';');
        output.x = parseNumber(s.substr(1, p1), parseError);
        output.y = parseNumber(s.substr(p1 + 1, p2), parseError);
        output.z = parseNumber(s.substr(p2 + 1, s.size() - 1), parseError);
    }

    return output;
}
