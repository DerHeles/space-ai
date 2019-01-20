#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "System.hpp"
#include <SFML/Graphics.hpp>

class RenderSystem : public System
{
public:
	RenderSystem(sf::RenderWindow *window, World *world);
	void update(sf::Time elapsedTime) override;
	void render(float timeProgressFactor) const;

private:
	sf::RenderWindow *m_window;
};
#endif