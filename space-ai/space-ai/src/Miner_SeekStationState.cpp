#include "Miner_SeekStationState.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "World.hpp"
#include "VectorHelper.hpp"

namespace miner
{
	SeekStationState::SeekStationState(StateMachine* pStateMachine, World *world)
		:
		State(pStateMachine),
		m_world(world)
	{
	}

	void SeekStationState::enter()
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));
			target->targetEntity = nullptr;

			for(auto e : m_world->getEntityList())
			{
				if((e->getComponentMask() & comp::MASK_GAMELOGIC) == comp::MASK_GAMELOGIC)
				{
					auto logic = static_cast<comp::GameLogic*>(e->getComponent(comp::MASK_GAMELOGIC));
					if (logic->role == "station_miner")
					{
						target->targetEntity = e;
						break;
					}
				}
			}
			if (target->targetEntity != nullptr)
			{
				ai->steering_pos = &m_seek;
				m_seek.setTarget(target->targetEntity);
			}
		}
	}

	void SeekStationState::update(sf::Time elapsedTime)
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto selfTrans = static_cast<comp::Transform*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TRANSFORM));
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));
			if(target->targetEntity != nullptr && (target->targetEntity->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
			{
				auto targetTrans = static_cast<comp::Transform*>(target->targetEntity->getComponent(comp::MASK_TRANSFORM));

				sf::Vector2f distanceVector = sf::Vector2f(selfTrans->x - targetTrans->x, selfTrans->y - targetTrans->y);
				if (vhelp::length(distanceVector) <= 100.f) //Station in Reichweite
				{
					if((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_STORAGE) == comp::MASK_STORAGE)
					{
						auto storage = static_cast<comp::Storage*>(m_pStateMachine->getParent()->getComponent(comp::MASK_STORAGE));
						storage->material = 0.f;
						storage->bar.setSize(sf::Vector2f(0.f, 10.f));
					}
					m_pStateMachine->changeToState("miner_searchForAsteroid");
				}
			}
		}
	}

	void SeekStationState::exit()
	{
		m_seek.setTarget(nullptr);
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			ai->steering_pos = nullptr;
		}
	}
}
