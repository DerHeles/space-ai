#include "Application.hpp"
#include <iostream>
#include "Configuration.hpp"
#include "MovementSystem.hpp"
#include "AISystem.hpp"
#include "AttackSystem.hpp"
#include "DeathSystem.hpp"
#include "PoliceSystem.hpp"
#include "SpawnSystem.hpp"

Application::Application(sf::ContextSettings settings)
	:
	m_window(sf::VideoMode(cfg::WINDOW_WIDTH, cfg::WINDOW_HEIGHT), cfg::WINDOW_TITLE, sf::Style::Default, settings),
	m_world(cfg::GAME_AREA_WIDTH, cfg::GAME_AREA_HEIGHT)
{

	m_systems.push_back(new AISystem(&m_world));
	m_systems.push_back(new MovementSystem(&m_world));
	m_systems.push_back(new AttackSystem(&m_world));

	PoliceSystem* ps = new PoliceSystem(&m_world);
	m_systems.push_back(ps);

	DeathSystem* ds = new DeathSystem(&m_world, ps);
	m_systems.push_back(ds);
	m_systems.push_back(new SpawnSystem(&m_world, ds));
	m_renderSystem = new RenderSystem(&m_window, &m_world);

	m_world.setDeathSystem(ds);
	m_world.setPoliceSystem(ps);
	m_world.init();
	m_view.reset(sf::FloatRect(0, 0, cfg::GAME_AREA_WIDTH, cfg::GAME_AREA_HEIGHT));

	if (!m_spaceTexture.loadFromFile("img/space.jpg"))
		std::cerr << "Application ctor error loading background texture!" << std::endl;

	m_spaceShape.setSize(sf::Vector2f(cfg::GAME_AREA_WIDTH, cfg::GAME_AREA_HEIGHT));
	m_spaceShape.setTexture(&m_spaceTexture);
}

Application::Application(unsigned int width, unsigned int height, sf::ContextSettings settings)
	:
	m_window(sf::VideoMode(width, height), cfg::WINDOW_TITLE, sf::Style::Default, settings),
	m_world(width, height)
{

	m_window.setVerticalSyncEnabled(true);

	m_systems.push_back(new MovementSystem(&m_world));
	m_renderSystem = new RenderSystem(&m_window, &m_world);

	m_world.init();
}

Application::~Application()
{
	for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
		delete *it;
	delete m_renderSystem;
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timePerFrame = sf::seconds(cfg::TIME_PER_FRAME);
	sf::Time stepTime;

	while(m_window.isOpen())
	{
		stepTime += clock.restart();
		while(stepTime >= timePerFrame)
		{
			processEvents();
			if(m_window.hasFocus())
				processInput();
			update(timePerFrame);

			stepTime -= timePerFrame;
		}
		render(stepTime.asSeconds() / cfg::TIME_PER_FRAME);
	}
}

void Application::update(sf::Time elapsedTime)
{
	for (auto system : m_systems)
		system->update(elapsedTime);
}

void Application::processEvents()
{
	sf::Event event;
	while(m_window.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				m_window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					m_window.close();
				else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
					m_view.move(0.f, -cfg::CAMERA_MOVE_OFFSET);
				else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
					m_view.move(0.f, cfg::CAMERA_MOVE_OFFSET);
				else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left)
					m_view.move(-cfg::CAMERA_MOVE_OFFSET, 0.f);
				else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right)
					m_view.move(cfg::CAMERA_MOVE_OFFSET, 0.f);
				break;
			case sf::Event::MouseWheelScrolled:
				if (event.mouseWheelScroll.delta > 0)
					m_view.zoom(0.9f);
				else if (event.mouseWheelScroll.delta < 0)
					m_view.zoom(1.1f);
				break;
		}
	}
}

void Application::processInput() const
{
	//do nothing
}

void Application::render(float timeProgressFactor)
{
	m_window.clear(sf::Color(10, 10, 100));
	m_window.setView(m_view);
	m_window.draw(m_spaceShape);
	m_renderSystem->render(timeProgressFactor);
	m_window.display();
}
