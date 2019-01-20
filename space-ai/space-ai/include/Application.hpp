#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SFML/Graphics.hpp>
#include "RenderSystem.hpp"
#include "World.hpp"

class Application
{
public:
	Application(sf::ContextSettings settings);
	Application(unsigned int width, unsigned int height, sf::ContextSettings settings);
	~Application();
	void run();

private:
	void update(sf::Time elapsedTime);
	void processEvents();
	void processInput() const;
	void render(float timeProgressFactor);

	sf::RenderWindow m_window;
	std::vector<System*> m_systems;
	RenderSystem *m_renderSystem;
	World m_world;
	sf::View m_view;

	sf::RectangleShape m_spaceShape;
	sf::Texture m_spaceTexture;
};
#endif