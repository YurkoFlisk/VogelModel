#include "Animation.h"

Animation::Animation(int lengthS, bool loop, bool paused)
	: m_animLength(lengthS), m_paused(paused),
	m_loop(loop), m_totalElapsed(0)
{}

void Animation::update(float elapsed)
{
	if (m_paused)
		return;
	m_totalElapsed += elapsed;
	if (m_totalElapsed > m_animLength)
		if (m_loop)
			m_totalElapsed = 0;
		else
			m_totalElapsed = m_animLength, m_paused = true;
}
