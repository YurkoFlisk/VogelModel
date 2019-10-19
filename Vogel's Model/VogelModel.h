#pragma once
#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include "InputManager.h"
#include "States.h"

class VogelModel
{
public:
	static constexpr int DEFAULT_WIDTH = 1251;
	static constexpr int DEFAULT_HEIGHT = 1001;
	static constexpr int FRAMERATE_LIMIT = 60;

	VogelModel(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);

	template<typename State>
	void changeState()
	{
		static_assert(std::is_base_of_v<StateBase, State>,
			"State should be derived from StateBase");
		delete m_currState.release(); // Release old state before constructing new
		m_currState = std::make_unique<State>(&m_context);
	}

	void run();
private:
	void update();
	void render();

	float m_elapsedSecs; // Time elapsed from last frame (s) 
	std::unique_ptr<StateBase> m_currState;
	InputManager m_inputManager;
	Context m_context;
	sf::Font m_arialFont;
	sf::Clock m_clock;
};
