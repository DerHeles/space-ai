#include "Wander.hpp"
#include "Components.hpp"
#include "Entity.hpp"

Wander::Wander()
	:
	m_wanderAngle(0.f),
	m_wanderAngularVelocity(0.f),
	m_maxWanderAngularVelocity(80.f),
	m_centerOffset(80.f),
	m_wanderRadius(8.f)
{

}

void Wander::apply(Entity* pSelf, sf::Time elapsedTime)
{
	sf::Vector2f t = calculateCircleCenter(pSelf) + calculateCenterToPointVector(pSelf);

	comp::Velocity* velo = dynamic_cast<comp::Velocity*>(pSelf->getComponent(comp::MASK_VELOCITY));
	comp::Transform* trans = dynamic_cast<comp::Transform*>(pSelf->getComponent(comp::MASK_TRANSFORM));
	comp::Acceleration* accel = dynamic_cast<comp::Acceleration*>(pSelf->getComponent(comp::MASK_ACCELERATION));

	if(velo && trans && accel)
	{
		sf::Vector2f currentVelocity = sf::Vector2f(velo->x, velo->y);
		sf::Vector2f currentPosition = sf::Vector2f(trans->x, trans->y);
		sf::Vector2f desiredVelocity = t - currentPosition;
		sf::Vector2f newAccel = desiredVelocity - currentVelocity;
		accel->x = newAccel.x;
		accel->y = newAccel.y;

		//update WanderAngle
		double wanderAngularAccel = (3.6*double(std::rand()) / double(RAND_MAX) - 1.8);
		m_wanderAngularVelocity += wanderAngularAccel * elapsedTime.asSeconds();

		if (std::abs(m_wanderAngularVelocity) > m_maxWanderAngularVelocity) {
			m_wanderAngularVelocity = m_wanderAngularVelocity > 0.f ? m_maxWanderAngularVelocity : -m_maxWanderAngularVelocity;
		}
		m_wanderAngle += m_wanderAngularVelocity * elapsedTime.asSeconds();
	}
}

sf::Vector2f Wander::calculateCircleCenter(Entity* pSelf)
{
	comp::Transform* trans = static_cast<comp::Transform*>(pSelf->getComponent(comp::MASK_TRANSFORM));
	if(trans)
	{
		sf::Vector2f currentPosition = sf::Vector2f(trans->x, trans->y);
		// calculate the direction we're facing from the rotation
		float currentRotation = 3.1415927f / 180.f * trans->rotation;
		sf::Vector2f faceDirection(std::cos(currentRotation), std::sin(currentRotation));
		// calculate the center of a circle right in front of you
		return currentPosition + m_centerOffset * faceDirection;
	}
	return sf::Vector2f();
}

sf::Vector2f Wander::calculateCenterToPointVector(Entity* pSelf)
{
	comp::Transform* trans = static_cast<comp::Transform*>(pSelf->getComponent(comp::MASK_TRANSFORM));
	if (trans)
	{
		float wanderAngleInWorld = m_wanderAngle + 3.1415927f / 180.f * trans->rotation;
		sf::Vector2f r = m_wanderRadius * sf::Vector2f(std::cos(wanderAngleInWorld), std::sin(wanderAngleInWorld));
		return r;
	}
	return sf::Vector2f();
}
