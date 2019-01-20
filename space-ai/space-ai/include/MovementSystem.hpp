#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include "System.hpp"

class MovementSystem : public System
{
public:
	MovementSystem(World *world);

	void update(sf::Time elapsedTime) override;
};
#endif