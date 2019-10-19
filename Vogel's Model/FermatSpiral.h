#pragma once
#include <cmath>
#include "misc.h"
#include "Drawable.h"

class FermatSpiral :
	public Drawable
{
public:
	enum class Parameter {
		AngleInc, FermatMul, FloretRadius, FloretCount
	};

	static constexpr float DEFAULT_FLORET_RADIUS = 7.0f;
	static constexpr float DEFAULT_FERMAT_MUL = 30.0f;
	static constexpr int DEFAULT_FLORET_COUNT = 100;
	static constexpr int NUM_COLORS = 5;
	static inline const float DEFAULT_ANGLE_INC = constants::PI * (3 - sqrt(5)); // radians
	static inline const float DEFAULT_ANGLE_INC_DEG = radToDeg(DEFAULT_ANGLE_INC); // degrees
	static inline const sf::Color colors[NUM_COLORS] = {
		sf::Color::Red,
		sf::Color::Green,
		sf::Color::Blue,
		sf::Color::Yellow,
		sf::Color::White
	};

	FermatSpiral(sf::Vector2f center = { 0, 0 }, float floretCount = DEFAULT_FLORET_COUNT,
		float floretRadius = DEFAULT_FLORET_RADIUS, float fermatMul = DEFAULT_FERMAT_MUL);

	inline float getFermatMul() const noexcept { return m_fermatMul; }
	inline float getFloretRadius() const noexcept { return m_floretRadius; }
	inline float getAngleInc() const noexcept { return m_angleInc; }
	inline int getFloretCount() const noexcept { return m_floretCount; }
	template<typename T = float>
	inline T getParam(Parameter param) const noexcept
	{
		switch (param)
		{
		case Parameter::AngleInc: return getAngleInc(); break;
		case Parameter::FermatMul: return getFermatMul(); break;
		case Parameter::FloretRadius: return getFloretRadius(); break;
		case Parameter::FloretCount: return getFloretCount(); break;
		}
	}
	inline void setFermatMul(float mul) noexcept { m_fermatMul = mul; }
	inline void setAngleInc(float inc) noexcept { m_angleInc = inc; }
	inline void setFloretRadius(float radius) noexcept { m_floretRadius = radius; }
	inline void setFloretCount(int count) noexcept { m_floretCount = count; }
	template<typename T>
	inline void setParam(Parameter param, T val) noexcept
	{
		switch (param)
		{
		case Parameter::AngleInc: setAngleInc(val); break;
		case Parameter::FermatMul: setFermatMul(val); break;
		case Parameter::FloretRadius: setFloretRadius(val); break;
		case Parameter::FloretCount: setFloretCount(val); break;
		}
	}
	// Defining function of fermat's spiral in polar coordinates
	inline float fermatFunction(float angle)
	{
		return m_fermatMul * sqrt(angle);
	}

	virtual void render(sf::RenderWindow& window) override;
private:
	int m_floretCount;
	sf::Vector2f m_center;
	float m_floretRadius; // pixels
	float m_angleInc; // radians
	float m_fermatMul; // multiplier in Fermat's function
};
