#pragma once
#include <unordered_map>
#include "FermatSpiral.h"
#include "Animation.h"

class IncreasingAngleAnimation
	: public Animation
{
	static constexpr float DEFAULT_ANIM_LENGTH = 25.0; // seconds
public:
	struct ParamEnds
	{
		// Returns value between start and end for given
		// fraction of 'way' from start to end
		inline float between(float fraction) const noexcept
		{
			return start + (end - start) * fraction;
		}

		float start;
		float end;
	};
	using Parameter = FermatSpiral::Parameter;
	using ParamEndsMap = std::unordered_map<Parameter, ParamEnds>;

	IncreasingAngleAnimation(sf::Vector2f center = { 0, 0 }, bool loop = false,
		float animLength = DEFAULT_ANIM_LENGTH);

	inline const ParamEnds& getBounds(Parameter param) const
	{
		auto it = m_params.find(param);
		if (it == m_params.end())
			throw std::runtime_error("Wrong parameter name");
		return it->second;
	}
	inline ParamEnds& getBounds(Parameter param)
	{
		return const_cast<ParamEnds&>(static_cast<
			const IncreasingAngleAnimation*>(this)->getBounds(param));
	}
	inline float getStart(Parameter param) const
	{
		return getBounds(param).start;
	}
	inline float getCurrent(Parameter param) const
	{
		return m_fermatSpiral.getParam(param);
	}
	inline float getEnd(Parameter param) const
	{
		return getBounds(param).end;
	}
	inline void setStart(Parameter param, float start)
	{
		getBounds(param).start = start;
	}
	inline void setEnd(Parameter param, float end)
	{
		getBounds(param).end = end;
	}
	inline void setBounds(Parameter param, float start, float end)
	{
		ParamEnds& ends = getBounds(param);
		ends.start = start;
		ends.end = end;
	}

	virtual void render(sf::RenderWindow& window) override;
private:
	FermatSpiral m_fermatSpiral;
	ParamEndsMap m_params;
};
