#include "Miner_WorkState.hpp"
#include "World.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "VectorHelper.hpp"

namespace miner
{
	WorkState::WorkState(StateMachine* pStateMachine, World *world)
		:
		StateMachine(pStateMachine->getParent(), pStateMachine),
		m_world(world)
	{
	}

	void WorkState::enter()
	{
		if (m_currentState.empty())
			changeToState(m_defaultState);
		else
			m_states.at(m_currentState)->enter();
	}

	void WorkState::update(sf::Time elapsedTime)
	{
		bool flee = false;
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
		{
			auto trans = static_cast<comp::Transform*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TRANSFORM));
			sf::Vector2f position = sf::Vector2f(trans->x, trans->y);

			for (auto e : m_world->getEntityList())
			{
				if ((e->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM
					&& (e->getComponentMask() & comp::MASK_GAMELOGIC) == comp::MASK_GAMELOGIC)
				{
					auto enemyTrans = static_cast<comp::Transform*>(e->getComponent(comp::MASK_TRANSFORM));
					auto logic = static_cast<comp::GameLogic*>(e->getComponent(comp::MASK_GAMELOGIC));
					if (logic->role == "pirate")
					{
						sf::Vector2f enemyPos = sf::Vector2f(enemyTrans->x, enemyTrans->y);
						sf::Vector2f distance = enemyPos - position;
						if (vhelp::length(distance) < 400.f)
						{
							flee = true;
							break;
						}
					}

				}
			}
		}
		if(flee)
		{
			m_pStateMachine->changeToState("miner_flee");
		}
		else
		{
			if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_HEALTH) == comp::MASK_HEALTH)
			{
				auto health = static_cast<comp::Health*>(m_pStateMachine->getParent()->getComponent(comp::MASK_HEALTH));
				if (health->hp < 70.f)
					m_pStateMachine->changeToState("miner_repair");
				else
					StateMachine::update(elapsedTime);
			}
		}
	}

	void WorkState::exit()
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));
			//after flee or repair a new target/asteroid should be searched
			target->targetEntity = nullptr;
			target->active = false;
		}
	}
}
