#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <cstdint>
#include <iostream>

constexpr uint16_t  WINDOW_X = 500;
constexpr uint16_t  WINDOW_Y = 500;
constexpr uint16_t  GRID_X = 20;
constexpr uint16_t  GRID_Y = 20;
constexpr bool      ENABLE_GRID_TEST_MOD = true;

enum THRUST_DIRECTION
{
        NONE=0,
        UP=1,
        LEFT=2,
        DOWN=4,
        RIGHT=16
};



const std::string         PATH_TEXTURES           = "../../ressources/textures/";
const std::string         PATH_SOUNDS           = "../../ressources/sounds/";

constexpr double    pi                      = 3.14159265358979;
constexpr double    tau                     = 2*pi;

constexpr uint8_t   NB_OF_VERTEX_ASTEROIDS  = 20;

constexpr float     MEAN_RADIUS_ASTEROIDS = 10.0;
constexpr float     MIN_MEAN_RADIUS_VARIATION_ASTEROIDS = 0.75; //MEAN_RADIUS_ASTEROIDS - this%MEAN_RADIUS_ASTEROIDS
constexpr float     MAX_MEAN_RADIUS_VARIATION_ASTEROIDS = 0.75; //MEAN_RADIUS_ASTEROIDS + this%MEAN_RADIUS_ASTEROIDS

constexpr float     ROUGHNESS_ASTEROIDS     = 1.5;
constexpr float     MIN_RADIUS_VARIATION_ASTEROIDS = 0.0;
constexpr float     MAX_RADIUS_VARIATION_ASTEROIDS = 0.5;

constexpr float     MAX_THRUST_VESSEL   = 1000;
constexpr float     FRICTION_VESSEL     = 5;
constexpr float     SPEED_LIMIT_VESSEL  = 400;

#endif //DEFINES_HPP
