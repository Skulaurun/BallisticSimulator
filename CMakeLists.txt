cmake_minimum_required(VERSION 3.12.0)

project(ShootingSimulator)

SET(CMAKE_CXX_STANDARD 23)
SET(CMAKE_C_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
    EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt
    GIT_TAG v3.15.0
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/entt
    OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.10.0
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Catch2
    OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(
    EnTT
    Catch2
)

add_subdirectory(src ${CMAKE_BINARY_DIR}/bin)
