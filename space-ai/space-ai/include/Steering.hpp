#ifndef POSITION_STEERING_HPP
#define POSITION_STEERING_HPP

#include <SFML/System/Time.hpp>

class Entity;

class PositionSteering
{
public:
	virtual void apply(Entity* pSelf, sf::Time elapsedTime = sf::Time::Zero) = 0;
};

class AngularSteering
{
public:
	virtual void apply(Entity* pSelf) = 0;
};

#endif