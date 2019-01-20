#include "Miner_DismantleAsteroid.hpp"
#include "Components.hpp"
#include "StateMachine.hpp"
#include "Entity.hpp"

namespace miner
{
	DismantleAsteroidState::DismantleAsteroidState(StateMachine* pStateMachine, DeathSystem* deathSystem)
		:
		State(pStateMachine)
	{
		deathSystem->addReceiver(this);
	}

	void DismantleAsteroidState::enter()
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));
			if(target->targetEntity == nullptr) //after flee --> look for new asteroid
			{
				m_pStateMachine->changeToState("miner_searchForAsteroid");
				return;
			}
			target->active = true;
			if((target->targetEntity->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
			{
				auto trans = static_cast<comp::Transform*>(target->targetEntity->getComponent(comp::MASK_TRANSFORM));
				target->line.setPoint2(sf::Vector2f(trans->x, trans->y));
			}
			if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_VELOCITY) == comp::MASK_VELOCITY)
			{
				auto velo = static_cast<comp::Velocity*>(m_pStateMachine->getParent()->getComponent(comp::MASK_VELOCITY));
				velo->x = 0.f;
				velo->y = 0.f;
				velo->angular = 0.f;
			}
		}
	}

	void DismantleAsteroidState::update(sf::Time elapsedTime)
	{
		//storage full --> go to station
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_STORAGE) == comp::MASK_STORAGE)
		{
			auto storage = static_cast<comp::Storage*>(m_pStateMachine->getParent()->getComponent(comp::MASK_STORAGE));
			if (storage->material == storage->maxMaterial)
				m_pStateMachine->changeToState("miner_seekStation");
		}
	}

	void DismantleAsteroidState::exit()
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));
			target->active = false;
			target->targetEntity = nullptr;
		}
	}

	void DismantleAsteroidState::receiveEvent(Entity* entity)
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));
			if(target->targetEntity == entity)
			{
				//asteroid dismantled & storage full --> go to station
				if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_STORAGE) == comp::MASK_STORAGE)
				{
					auto storage = static_cast<comp::Storage*>(m_pStateMachine->getParent()->getComponent(comp::MASK_STORAGE));
					if (storage->material == storage->maxMaterial)
					{
						m_pStateMachine->changeToState("miner_seekStation");
						return;
					}
				}
				//asteroid dismantled & storage not full --> search for next asteroid
				m_pStateMachine->changeToState("miner_searchForAsteroid");
			}
		}
	}
}
