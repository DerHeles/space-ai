#include "Flee.hpp"
#include "Entity.hpp"
#include "VectorHelper.hpp"

Flee::Flee()
{
}

void Flee::addEnemy(Entity* enemy)
{
	for (auto e : m_enemies)
		if (e == enemy)
			return;
	m_enemies.push_back(enemy);
}

bool Flee::removeEnemy(Entity* enemy)
{
	for(auto it = m_enemies.begin(); it != m_enemies.end(); ++it)
		if((*it) == enemy)
		{
			m_enemies.erase(it);
			break;
		}
	return m_enemies.empty();
}

void Flee::removeAllEnemies()
{
	m_enemies.clear();
}

void Flee::apply(Entity* pSelf, sf::Time elapsedTime)
{
	if(true)
	if ((pSelf->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM
		&& (pSelf->getComponentMask() & comp::MASK_VELOCITY) == comp::MASK_VELOCITY
		&& (pSelf->getComponentMask() & comp::MASK_ACCELERATION) == comp::MASK_ACCELERATION)
	{
		auto trans = static_cast<comp::Transform*>(pSelf->getComponent(comp::MASK_TRANSFORM));
		auto velo = static_cast<comp::Velocity*>(pSelf->getComponent(comp::MASK_VELOCITY));
		auto accel = static_cast<comp::Acceleration*>(pSelf->getComponent(comp::MASK_ACCELERATION));

		sf::Vector2f currentPosition = sf::Vector2f(trans->x, trans->y);
		sf::Vector2f currentVelocity = sf::Vector2f(velo->x, velo->y);
		sf::Vector2f steeringTotal;

		for (auto e : m_enemies)
		{
			if ((e->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
			{
				auto enemyTrans = static_cast<comp::Transform*>(e->getComponent(comp::MASK_TRANSFORM));

				sf::Vector2f desiredPosition = sf::Vector2f(enemyTrans->x, enemyTrans->y);

				sf::Vector2f desiredVelocity = currentPosition - desiredPosition;
				vhelp::normalize(&desiredVelocity);
				desiredVelocity *= velo->y_max;

				sf::Vector2f steering = desiredVelocity - currentVelocity;
				vhelp::normalize(&steering);
				steering *= velo->y_max;

				steeringTotal += steering;
			}
		}

		vhelp::normalize(&steeringTotal);
		steeringTotal *= accel->max;
		accel->x = steeringTotal.x;
		accel->y = steeringTotal.y;
	}

}