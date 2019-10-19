#include "FermatSpiral.h"
#include "misc.h"

FermatSpiral::FermatSpiral(sf::Vector2f center, float floretCount,
	float floretRadius, float fermatMul)
	: m_floretCount(floretCount), m_floretRadius(floretRadius),
	m_fermatMul(fermatMul), m_angleInc(DEFAULT_ANGLE_INC), m_center(center)
{}

void FermatSpiral::render(sf::RenderWindow& window)
{
	sf::CircleShape floret(m_floretRadius);
	floret.setOrigin(m_floretRadius / 2, m_floretRadius / 2);
	float currentAngle = 0.0;
	for (int i = 0; i < m_floretCount; ++i)
	{
		currentAngle = i * m_angleInc;
		floret.setPosition(m_center +
			polarToVec2f(currentAngle, fermatFunction(currentAngle)));
		floret.setFillColor(colors[i % NUM_COLORS]);
		window.draw(floret);
	}
}
