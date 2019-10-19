#pragma once
#include "SFML/Graphics.hpp"

class Drawable
{
public:
	virtual ~Drawable() = default;

	virtual void update(float elapsed) {};
	virtual void render(sf::RenderWindow& window) = 0;
};
