#include "States.h"
#include "IncreasingAngleAnimation.h"
#include "VogelModel.h"

StateBase::~StateBase()
{
	// Clean ALL GUI on state destruction (maybe temporarily)
	m_context->gui.removeAllWidgets();
}

StateStatic::StateStatic(Context* context)
	: StateBase(context)
{
	tgui::Gui& gui = m_context->gui;

	auto layout = tgui::Group::create();
	layout->setSize({ GUI_WIDTH, "100%" });
	layout->setPosition({"&.width - " + std::to_string(GUI_WIDTH), "0"});
	int curY(0);

	auto animButton = tgui::Button::create("Animation");
	animButton->setPosition(0, curY);
	animButton->connect("pressed", &StateStatic::sToAnim, this);
	layout->add(animButton);
	curY += animButton->getSize().y + GUI_GAP;

	auto defaultAngleButton = tgui::Button::create("Vogel's angle");
	defaultAngleButton->setPosition(0, curY);
	defaultAngleButton->connect("pressed", &StateStatic::sDefaultAngle, this);
	layout->add(defaultAngleButton);
	curY += defaultAngleButton->getSize().y + GUI_GAP;

	auto& angleEditor = m_paramEditors[Parameter::AngleInc] = ParamEditor{
		.displayName = "Angle inc (degrees)",
		.label = tgui::Label::create("Angle inc: Default"),
		.slider = tgui::Slider::create(0, 360)
	};
	angleEditor.addToLayout(layout, 0, curY);
	curY += angleEditor.getHeight() + GUI_GAP;

	auto& multEditor = m_paramEditors[Parameter::FermatMul] = ParamEditor{
		.displayName = "Fermat mul",
		.label = tgui::Label::create("Fermat mul: Default"),
		.slider = tgui::Slider::create(1, 40)
	}; 
	multEditor.addToLayout(layout, 0, curY);
	curY += multEditor.getHeight() + GUI_GAP;

	auto& radiusEditor = m_paramEditors[Parameter::FloretRadius] = ParamEditor{
		.displayName = "Floret radius",
		.label = tgui::Label::create("Floret radius: Default"),
		.slider = tgui::Slider::create(1, 10)
	};
	radiusEditor.addToLayout(layout, 0, curY);
	curY += radiusEditor.getHeight() + GUI_GAP;

	auto& countEditor = m_paramEditors[Parameter::FloretCount] = ParamEditor{
		.displayName = "Floret count",
		.label = tgui::Label::create("Floret count: Default"),
		.slider = tgui::Slider::create(1, 20000)
	};
	countEditor.addToLayout(layout, 0, curY);
	curY += countEditor.getHeight() + GUI_GAP;

	m_spiral = new FermatSpiral(m_context->window.getView().getCenter());

	for (const auto& [param, editor] : m_paramEditors)
	{
		float value = m_spiral->getParam(param);
		if (param == Parameter::AngleInc)
			value = radToDeg(value);
		editor.slider->setValue(value);
		editor.slider->connect("valueChanged", [this, param](float value) {
			return sValueChanged(param, value); });
	}

	gui.add(layout);
}

StateStatic::~StateStatic()
{
	delete m_spiral;
}

void StateStatic::update(float elapsedSecs)
{
	m_spiral->update(elapsedSecs);
}

void StateStatic::render()
{
	m_spiral->render(m_context->window);
}

void StateStatic::sDefaultAngle()
{
	// This will NOT call sValueChanged, so need to call manually
	m_paramEditors[Parameter::AngleInc].slider->setValue(FermatSpiral::DEFAULT_ANGLE_INC_DEG);
	sValueChanged(Parameter::AngleInc, FermatSpiral::DEFAULT_ANGLE_INC_DEG);
}

void StateStatic::sToAnim()
{
	m_context->vogel->changeState<StateAnim>();
}

void StateStatic::sValueChanged(Parameter param, float curVal)
{
	ParamEditor& editor = m_paramEditors[param];
	editor.label->setText(editor.displayName + ":" + std::to_string(curVal));

	if (param == Parameter::AngleInc)
		curVal = degToRad(curVal);
	m_spiral->setParam(param, curVal);
}

StateAnim::StateAnim(Context* context)
	: StateBase(context)
{
	tgui::Gui& gui = m_context->gui;

	auto layout = tgui::Group::create();
	layout->setSize({ GUI_WIDTH, "100%" });
	layout->setPosition({ "&.width - " + std::to_string(GUI_WIDTH), "0" });
	int curY(0);

	auto staticButton = tgui::Button::create("Static");
	staticButton->setPosition(0, curY);
	staticButton->connect("pressed", &StateAnim::sToStatic, this);
	layout->add(staticButton);
	curY += staticButton->getSize().y + GUI_GAP;

	auto animLengthL = tgui::Label::create("Anim length (s):");
	animLengthL->setPosition(0, curY);
	m_animLengthEB = tgui::EditBox::create();
	m_animLengthEB->setPosition(GUI_WIDTH / 2, curY);
	m_animLengthEB->setInputValidator(tgui::EditBox::Validator::Float);
	m_animLengthEB->setMaximumCharacters(10);
	layout->add(animLengthL);
	layout->add(m_animLengthEB);
	curY += std::max(animLengthL->getSize().y, m_animLengthEB->getSize().y) + GUI_GAP;

	m_animTimeL = tgui::Label::create("Time elapsed (s): 0");
	m_animTimeL->setPosition(0, curY);
	layout->add(m_animTimeL);
	curY += m_animTimeL->getSize().y + GUI_GAP;

	auto startButton = tgui::Button::create("Start");
	startButton->setPosition(0, curY);
	startButton->connect("pressed", &StateAnim::sStart, this);
	layout->add(startButton);
	curY += startButton->getSize().y + GUI_GAP;

	auto pauseButton = tgui::Button::create("Pause");
	pauseButton->setPosition(0, curY);
	pauseButton->connect("pressed", &StateAnim::sPause, this);
	layout->add(pauseButton);
	curY += pauseButton->getSize().y + GUI_GAP;

	auto stopButton = tgui::Button::create("Stop");
	stopButton->setPosition(0, curY);
	stopButton->connect("pressed", &StateAnim::sStop, this);
	layout->add(stopButton);
	curY += stopButton->getSize().y + GUI_GAP;

	auto& angleEditor = m_paramRangeEditors[Parameter::AngleInc] = ParamRangeEditor{
		.displayName = "Angle inc (degrees)",
		.label = tgui::Label::create("Angle inc: Default"),
		.slider = tgui::RangeSlider::create(0, 360)
	};
	angleEditor.addToLayout(layout, 0, curY);
	curY += angleEditor.getHeight() + GUI_GAP;

	auto& multEditor = m_paramRangeEditors[Parameter::FermatMul] = ParamRangeEditor{
		.displayName = "Fermat mul",
		.label = tgui::Label::create("Fermat mul: Default"),
		.slider = tgui::RangeSlider::create(1, 40)
	};
	multEditor.addToLayout(layout, 0, curY);
	curY += multEditor.getHeight() + GUI_GAP;

	auto& radiusEditor = m_paramRangeEditors[Parameter::FloretRadius] = ParamRangeEditor{
		.displayName = "Floret radius",
		.label = tgui::Label::create("Floret radius: Default"),
		.slider = tgui::RangeSlider::create(1, 10)
	};
	radiusEditor.addToLayout(layout, 0, curY);
	curY += radiusEditor.getHeight() + GUI_GAP;

	auto& countEditor = m_paramRangeEditors[Parameter::FloretCount] = ParamRangeEditor{
		.displayName = "Floret count",
		.label = tgui::Label::create("Floret count: Default"),
		.slider = tgui::RangeSlider::create(1, 20000)
	};
	countEditor.addToLayout(layout, 0, curY);
	curY += countEditor.getHeight() + GUI_GAP;

	gui.add(layout);

	m_animation = new IncreasingAngleAnimation(m_context->window.getView().getCenter());
	m_animLengthEB->setDefaultText(std::to_string(m_animation->getAnimLength()));
	m_animLengthEB->setText(m_animLengthEB->getDefaultText());

	for (const auto& [param, editor] : m_paramRangeEditors)
	{
		float start = m_animation->getStart(param), end = m_animation->getEnd(param);
		if (param == Parameter::AngleInc)
			start = radToDeg(start), end = radToDeg(end);
		editor.slider->setSelectionStart(start);
		editor.slider->setSelectionEnd(end);
		editor.slider->connect("RangeChanged", [this, param](float begin, float end) {
			sRangeChanged(param, begin, end); });
	}
}

StateAnim::~StateAnim()
{
	delete m_animation;
}

void StateAnim::update(float elapsedSecs)
{
	m_animation->update(elapsedSecs);
	for (auto& [param, editor] : m_paramRangeEditors)
	{
		float curValue = m_animation->getCurrent(param);
		if (param == Parameter::AngleInc)
			curValue = radToDeg(curValue);
		editor.label->setText(editor.displayName + ":"
			+ std::to_string(curValue));
	}
	m_animTimeL->setText("Time elapsed (s): "
		+ std::to_string(m_animation->getTotalElapsed()));
}

void StateAnim::render()
{
	m_animation->render(m_context->window);
}

void StateAnim::sToStatic()
{
	m_context->vogel->changeState<StateStatic>();
}

void StateAnim::sStart()
{
	m_animLengthEB->setEnabled(false);
	m_animation->setAnimLength(std::stof(m_animLengthEB->getText().toAnsiString()));
	m_animation->resume();
}

void StateAnim::sPause()
{
	m_animation->pause();
}

void StateAnim::sStop()
{
	m_animLengthEB->setEnabled(true);
	m_animation->stop();
}

void StateAnim::sRangeChanged(Parameter param, float start, float end)
{
	if (param == Parameter::AngleInc)
		start = degToRad(start), end = degToRad(end);
	m_animation->setBounds(param, start, end);
}

void StateStatic::ParamEditor::addToLayout(tgui::Group::Ptr group, float x, float y)
{
	label->setPosition(x, y);
	slider->setPosition(x, y + label->getSize().y + LABEL_SLIDER_GAP);
	group->add(label);
	group->add(slider);
}

void StateAnim::ParamRangeEditor::addToLayout(tgui::Group::Ptr group, float x, float y)
{
	label->setPosition(x, y);
	slider->setPosition(x, y + label->getSize().y + LABEL_SLIDER_GAP);
	group->add(label);
	group->add(slider);
}
