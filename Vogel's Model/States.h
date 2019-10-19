#pragma once
#include <TGUI/TGUI.hpp>
#include "FermatSpiral.h"

struct Context
{
	class VogelModel* vogel;
	sf::RenderWindow window;
	tgui::Gui gui;
};

class StateBase
{
public:
	StateBase(Context* context)
		: m_context(context)
	{}
	virtual ~StateBase();

	virtual void update(float elapsedSecs) = 0;
	virtual void render() = 0;
protected:
	Context* m_context;
};

class StateStatic
	: public StateBase
{
public:
	struct ParamEditor
	{
		static constexpr float LABEL_SLIDER_GAP = 10;

		inline float getHeight(void) const
		{
			return label->getSize().y + LABEL_SLIDER_GAP + slider->getSize().y;
		}
		void addToLayout(tgui::Group::Ptr group, float x, float y);

		std::string displayName;
		tgui::Label::Ptr label;
		tgui::Slider::Ptr slider;
	};
	using Parameter = FermatSpiral::Parameter;
	using ParamEditors = std::unordered_map<Parameter, ParamEditor>;

	static constexpr float GUI_WIDTH = 200;
	static constexpr float GUI_GAP = 30;

	StateStatic(Context* context);
	~StateStatic();

	virtual void update(float elapsedSecs) override;
	virtual void render() override;

	void sToAnim();
	void sDefaultAngle();
	void sValueChanged(Parameter param, float curVal);
private:
	ParamEditors m_paramEditors;
	FermatSpiral* m_spiral;
};

class StateAnim
	: public StateBase
{
public:
	struct ParamRangeEditor
	{
		static constexpr float LABEL_SLIDER_GAP = 10;

		inline float getHeight(void) const
		{
			return label->getSize().y + LABEL_SLIDER_GAP + slider->getSize().y;
		}
		void addToLayout(tgui::Group::Ptr group, float x, float y);

		std::string displayName;
		tgui::Label::Ptr label;
		tgui::RangeSlider::Ptr slider;
	};
	using Parameter = FermatSpiral::Parameter;
	using ParamRangeEditors = std::unordered_map<Parameter, ParamRangeEditor>;

	static constexpr float GUI_WIDTH = 200;
	static constexpr float GUI_GAP = 30;

	StateAnim(Context* context);
	~StateAnim();

	virtual void update(float elapsedSecs) override;
	virtual void render() override;

	void sToStatic();
	void sStart();
	void sPause();
	void sStop();
	void sRangeChanged(Parameter param, float start, float end);
private:
	tgui::EditBox::Ptr m_animLengthEB;
	tgui::Label::Ptr m_animTimeL;
	ParamRangeEditors m_paramRangeEditors;
	class IncreasingAngleAnimation* m_animation;
};