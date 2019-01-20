#include "Pirate_WanderState.hpp"
#include "World.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "VectorHelper.hpp"

namespace pirate
{
	WanderState::WanderState(StateMachine* pStateMachine, World* world)
		:
		State(pStateMachine),
		m_world(world)
	{
	}

	void WanderState::enter()
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI)
		{
			comp::AI *ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			ai->steering_pos = &m_wander;
		}
	}

	void WanderState::update(sf::Time elapsedTime)
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			comp::Transform *selfTrans = static_cast<comp::Transform*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TRANSFORM));
			comp::Target *selfTarget = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));


			//miner nearby
			Entity *enemy = nullptr;
			float distance = 350.f; //maxrange

			for (auto e : m_world->getEntityList())
			{
				if (e != m_pStateMachine->getParent() && (e->getComponentMask() & comp::MASK_GAMELOGIC) == comp::MASK_GAMELOGIC && (e->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
				{
					comp::GameLogic *logic = static_cast<comp::GameLogic*>(e->getComponent(comp::MASK_GAMELOGIC));
					comp::Transform *trans = static_cast<comp::Transform*>(e->getComponent(comp::MASK_TRANSFORM));
					if (logic->role == "police")
					{
						sf::Vector2f distanceVector = sf::Vector2f(selfTrans->x - trans->x, selfTrans->y - trans->y);
						if (vhelp::length(distanceVector) < distance)
						{
							distance = vhelp::length(distanceVector);
							enemy = e;
						}
					}
				}
			}
			if (enemy != nullptr)
			{
				selfTarget->targetEntity = enemy;
				m_pStateMachine->changeToState("pirate_fightPolice");
				return;
			}

			for (auto e : m_world->getEntityList())
			{
				if (e != m_pStateMachine->getParent() && (e->getComponentMask() & comp::MASK_GAMELOGIC) == comp::MASK_GAMELOGIC && (e->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
				{
					comp::GameLogic *logic = static_cast<comp::GameLogic*>(e->getComponent(comp::MASK_GAMELOGIC));
					comp::Transform *trans = static_cast<comp::Transform*>(e->getComponent(comp::MASK_TRANSFORM));
					if (logic->role == "miner")
					{
						sf::Vector2f distanceVector = sf::Vector2f(selfTrans->x - trans->x, selfTrans->y - trans->y);
						if (vhelp::length(distanceVector) < distance)
						{
							distance = vhelp::length(distanceVector);
							enemy = e;
						}
					}
				}
			}
			if (enemy != nullptr)
			{
				selfTarget->targetEntity = enemy;
				m_pStateMachine->changeToState("pirate_killMiner");
			}

		}
	}

	void WanderState::exit()
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI)
		{
			comp::AI *ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			ai->steering_pos = nullptr;
		}
	}
}
