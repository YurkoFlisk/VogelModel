#include "VogelModel.h"
#include "States.h"

VogelModel::VogelModel(int width, int height)
	: m_context{
		.vogel = this,
		.window = sf::RenderWindow(sf::VideoMode(width, height), "Vogel's Model",
			sf::Style::Default, sf::ContextSettings(0, 0, 8)),
		.gui = tgui::Gui(m_context.window) },
	m_elapsedSecs(0)
{
	changeState<StateStatic>();
	m_context.window.setVerticalSyncEnabled(true);
	// m_context.window.setFramerateLimit(FRAMERATE_LIMIT);
	m_arialFont.loadFromFile("arial.ttf");
}

void VogelModel::run()
{
	sf::RenderWindow& window = m_context.window;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (m_context.gui.handleEvent(event))
				continue;
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				m_inputManager.keyInput(event.key.code);
				break;
			}
		}

		sf::Time elapsed = m_clock.restart();
		m_elapsedSecs = elapsed.asSeconds();

		update();

		render();
	}
}

void VogelModel::update()
{
	m_currState->update(m_elapsedSecs);
}

void VogelModel::render()
{
	sf::RenderWindow& window = m_context.window;
	window.clear(sf::Color::Black);

	m_currState->render();
	m_context.gui.draw();

	window.display();
}
