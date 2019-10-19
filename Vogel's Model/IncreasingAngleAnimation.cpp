#include "IncreasingAngleAnimation.h"

IncreasingAngleAnimation::IncreasingAngleAnimation(sf::Vector2f center,
	bool loop, float animLength)
	: Animation(animLength, loop), m_fermatSpiral(center)
{
	m_params[Parameter::AngleInc] = { .start = 0,.end = 2 * constants::PI };
	m_params[Parameter::FloretRadius] = { // Unanimated
		.start = m_fermatSpiral.getFloretRadius(),
		.end = m_fermatSpiral.getFloretRadius() };
	m_params[Parameter::FermatMul] = { // Unanimated
		.start = m_fermatSpiral.getFermatMul(),
		.end = m_fermatSpiral.getFermatMul() };
	m_params[Parameter::FloretCount] = { // Unanimated
		.start = (float)m_fermatSpiral.getFloretCount(),
		.end = (float)m_fermatSpiral.getFloretCount()};
}

void IncreasingAngleAnimation::render(sf::RenderWindow& window)
{
	const float fraction = getFractionElapsed();

	for (const auto& [param, ends] : m_params)
		m_fermatSpiral.setParam(param, ends.between(fraction));

	m_fermatSpiral.render(window);
}
