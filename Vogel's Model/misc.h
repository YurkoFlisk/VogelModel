#pragma once
#include <SFML/Graphics.hpp>

namespace constants
{
	constexpr double PI = 3.14159265358979323846;
}

// Converts polar coordinates to cartesian ones
inline sf::Vector2f polarToVec2f(float angle, float radius)
{
	return sf::Vector2f(radius * cos(angle), radius * sin(angle));
}
// Converts radians to degrees
constexpr inline float radToDeg(float angle)
{
	return 180.0 / constants::PI * angle;
}
// Converts degrees to radians
constexpr inline float degToRad(float angle)
{
	return constants::PI / 180.0 * angle;
}