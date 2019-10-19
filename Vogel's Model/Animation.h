#pragma once
#include "Drawable.h"

class Animation
	: public Drawable
{
public:
	Animation(int lengthS = 10, bool loop = false, bool paused = true);

	inline float getTotalElapsed() const noexcept { return m_totalElapsed; }
	inline float getAnimLength() const noexcept { return m_animLength; }
	inline float getFractionElapsed() const noexcept { return getTotalElapsed() / getAnimLength(); }
	inline bool isLoop() const noexcept { return m_loop; }
	inline bool isPaused() const noexcept { return m_paused; }

	inline void setAnimLength(float length) noexcept { m_animLength = length; };
	inline void pause() noexcept { m_paused = true; }
	inline void resume() noexcept { m_paused = false; }
	inline void stop() noexcept { m_paused = true; m_totalElapsed = 0.0f; }

	virtual void update(float elapsed) override;
private:
	bool m_paused; // Whether animation is paused
	bool m_loop; // Whether animation begins again after the end
	float m_totalElapsed; // Total time elapsed in animation (excluding pauses) (s)
	float m_animLength; // Length of animation (s)
};
