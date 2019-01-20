#include "Police_WanderState.hpp"
#include "World.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "VectorHelper.hpp"

namespace police
{
	WanderState::WanderState(StateMachine* pStateMachine, World* world, PatrolState* const patrolState)
		:
		State(pStateMachine),
		m_world(world),
		m_patrolState(patrolState)
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
		if (m_patrolState->patrolTimeExpired())
			m_pStateMachine->changeToState("police_returnToStation");
		else
		{
			if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
			{
				comp::Transform *selfTrans = static_cast<comp::Transform*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TRANSFORM));
				comp::Target *selfTarget = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));

				//pirate nearby
				Entity *pirate = nullptr;
				float distance = 350.f; //maxrange

				for (auto e : m_world->getEntityList())
				{
					if (e != m_pStateMachine->getParent() && (e->getComponentMask() & comp::MASK_GAMELOGIC) == comp::MASK_GAMELOGIC && (e->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
					{
						comp::GameLogic *logic = static_cast<comp::GameLogic*>(e->getComponent(comp::MASK_GAMELOGIC));
						comp::Transform *trans = static_cast<comp::Transform*>(e->getComponent(comp::MASK_TRANSFORM));
						if (logic->role == "pirate")
						{
							sf::Vector2f distanceVector = sf::Vector2f(selfTrans->x - trans->x, selfTrans->y - trans->y);
							if (vhelp::length(distanceVector) < distance)
							{
								distance = vhelp::length(distanceVector);
								pirate = e;
							}
						}
					}
				}
				if (pirate != nullptr)
				{
					selfTarget->targetEntity = pirate;
					m_pStateMachine->changeToState("police_fight");
				}

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
