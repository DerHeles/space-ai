#ifndef WANDER_HPP
#define WANDER_HPP

#include "Steering.hpp"
#include <SFML/System/Vector2.hpp>

class Wander : public PositionSteering
{
public:
	Wander();
	void apply(Entity* pSelf, sf::Time elapsedTime) override;

private:
	sf::Vector2f calculateCircleCenter(Entity* pSelf);
	sf::Vector2f calculateCenterToPointVector(Entity* pSelf);

	float m_wanderAngle;
	float m_wanderAngularVelocity;
	float m_maxWanderAngularVelocity;
	float m_centerOffset;
	float m_wanderRadius;

};
#endif