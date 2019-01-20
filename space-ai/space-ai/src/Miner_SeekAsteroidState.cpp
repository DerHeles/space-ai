#include "Miner_SeekAsteroidState.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "Seek.hpp"
#include "VectorHelper.hpp"

namespace miner
{
	SeekAsteroidState::SeekAsteroidState(StateMachine* pStateMachine, DeathSystem* deathSystem)
		:
		State(pStateMachine)
	{
		deathSystem->addReceiver(this);
	}

	void SeekAsteroidState::enter()
	{
		if((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));
			if (target->targetEntity == nullptr) //nach flee soll asteroid neu gesucht werden
			{
				m_pStateMachine->changeToState("miner_searchForAsteroid");
				return;
			}
			ai->steering_pos = &m_seek;
			m_seek.setTarget(target->targetEntity);
		}
	}

	void SeekAsteroidState::update(sf::Time elapsedTime)
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto selfTrans = static_cast<comp::Transform*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TRANSFORM));
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));

			if((target->targetEntity->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
			{
				auto targetTrans = static_cast<comp::Transform*>(target->targetEntity->getComponent(comp::MASK_TRANSFORM));

				sf::Vector2f distanceVector = sf::Vector2f(selfTrans->x - targetTrans->x, selfTrans->y - targetTrans->y);
				if (vhelp::length(distanceVector) <= 200.f) //Asteroid in Reichweite
				{
					if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_VELOCITY) == comp::MASK_VELOCITY)
					{
						auto velo = static_cast<comp::Velocity*>(m_pStateMachine->getParent()->getComponent(comp::MASK_VELOCITY));
						velo->x = 0.f;
						velo->y = 0.f;
						velo->angular = 0.f;
					}
					if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_ACCELERATION) == comp::MASK_ACCELERATION)
					{
						auto accel = static_cast<comp::Acceleration*>(m_pStateMachine->getParent()->getComponent(comp::MASK_ACCELERATION));
						accel->x = 0.f;
						accel->y = 0.f;
					}
					//m_seek.setTarget(nullptr);
					m_pStateMachine->changeToState("miner_dismantleAsteroid");
				}
			}
		}
	}

	void SeekAsteroidState::exit()
	{
		m_seek.setTarget(nullptr);
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			ai->steering_pos = nullptr;
		}
	}

	void SeekAsteroidState::receiveEvent(Entity* entity)
	{
		if(m_seek.getTarget() == entity)
		{
			m_pStateMachine->changeToState("miner_searchForAsteroid");
		}
	}
}
