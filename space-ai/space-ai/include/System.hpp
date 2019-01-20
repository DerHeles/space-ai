#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <SFML/System/Time.hpp>

class World;

class System
{
public:
	System(World *world);
	virtual ~System()
	{}

	virtual void update(sf::Time elapsedTime) = 0;

protected:
	World *m_world;
};
#endif