#include "Seek.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include "VectorHelper.hpp"

Seek::Seek()
	:
	m_pTarget(nullptr)
{
}

Seek::Seek(Entity* pTarget)
	:
	m_pTarget(pTarget)
{
}

void Seek::apply(Entity* pSelf, sf::Time elapsedTime)
{
	if(m_pTarget != nullptr)
		if((pSelf->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM && (pSelf->getComponentMask() & comp::MASK_VELOCITY) == comp::MASK_VELOCITY
			&& (pSelf->getComponentMask() & comp::MASK_ACCELERATION) == comp::MASK_ACCELERATION && (m_pTarget->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
		{
			auto selfTrans = static_cast<comp::Transform*>(pSelf->getComponent(comp::MASK_TRANSFORM));
			auto targetTrans = static_cast<comp::Transform*>(m_pTarget->getComponent(comp::MASK_TRANSFORM));
			auto velo = static_cast<comp::Velocity*>(pSelf->getComponent(comp::MASK_VELOCITY));
			auto accel = static_cast<comp::Acceleration*>(pSelf->getComponent(comp::MASK_ACCELERATION));

			sf::Vector2f currentPosition = sf::Vector2f(selfTrans->x, selfTrans->y);
			sf::Vector2f currentVelocity = sf::Vector2f(velo->x, velo->y);

			sf::Vector2f desiredPosition = sf::Vector2f(targetTrans->x, targetTrans->y);
			sf::Vector2f desiredVelocity = desiredPosition - currentPosition;

			float distance = vhelp::length(desiredVelocity);
			//nahe am Ziel - langsamer werden
			if(distance < 350.f)
			{
				//extrem nahe am Ziel - stehen bleiben
				if(distance < 10.f)
				{
					accel->x = 0.f;
					accel->y = 0.f;
					velo->x = 0.f;
					velo->y = 0.f;
					return;
				}
				vhelp::normalize(&desiredVelocity);
				desiredVelocity *= velo->y_max;
				//Abbremsung ist linear zur Entfernung abhängig
				desiredVelocity *= (distance / 300.f);

				//sehr nahe am Ziel - Bremseffekt verstärken
				if(distance < 100.f)
				{
					desiredVelocity *= 0.3f;
				}
			}
			else
			{
				vhelp::normalize(&desiredVelocity);
				desiredVelocity *= velo->y_max;
			}

			sf::Vector2f steering = desiredVelocity - currentVelocity;
			if(vhelp::length(steering) > accel->max)
			{
				vhelp::normalize(&steering);
				steering *= accel->max;
			}
			accel->x = steering.x;
			accel->y = steering.y;
		}
}

Entity* const Seek::getTarget() const
{
	return  m_pTarget;
}

void Seek::setTarget(Entity* pTarget)
{
	m_pTarget = pTarget;
}
