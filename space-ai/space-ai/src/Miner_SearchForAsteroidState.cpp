#include "Miner_SearchForAsteroidState.hpp"
#include "StateMachine.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include "World.hpp"
#include "VectorHelper.hpp"

namespace miner
{
	SearchForAsteroidState::SearchForAsteroidState(StateMachine* pStateMachine, World *world)
		:
		State(pStateMachine),
		m_world(world)
	{
	}

	void SearchForAsteroidState::enter()
	{
		//storage full --> go to station
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_STORAGE) == comp::MASK_STORAGE)
		{
			auto storage = static_cast<comp::Storage*>(m_pStateMachine->getParent()->getComponent(comp::MASK_STORAGE));
			if (storage->material == storage->maxMaterial)
			{
				m_pStateMachine->changeToState("miner_seekStation");
				return;
			}
		}
		comp::AI *ai = dynamic_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
		if(ai)
		{
			ai->steering_pos = &m_wander;
		}
	}

	void SearchForAsteroidState::update(sf::Time elapsedTime)
	{
		if((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			comp::Transform *selfTrans = static_cast<comp::Transform*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TRANSFORM));
			comp::Target *selfTarget = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));


			//asteroid nearby
			Entity *asteroid = nullptr;
			float distance = 400.f; //maxrange

			for (auto e : m_world->getEntityList())
			{
				if (e != m_pStateMachine->getParent() && (e->getComponentMask() & comp::MASK_GAMELOGIC) == comp::MASK_GAMELOGIC && (e->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
				{
					comp::GameLogic *logic = static_cast<comp::GameLogic*>(e->getComponent(comp::MASK_GAMELOGIC));
					comp::Transform *trans = static_cast<comp::Transform*>(e->getComponent(comp::MASK_TRANSFORM));
					if(logic->role == "asteroid")
					{
						sf::Vector2f distanceVector = sf::Vector2f(selfTrans->x - trans->x, selfTrans->y - trans->y);
						if(vhelp::length(distanceVector) < distance)
						{
							distance = vhelp::length(distanceVector);
							asteroid = e;
						}
					}
				}
			}
			if(asteroid != nullptr)
			{
				selfTarget->targetEntity = asteroid;
				m_pStateMachine->changeToState("miner_seekAsteroid");
			}

		}

	}

	void SearchForAsteroidState::exit()
	{
		if((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI)
		{
			auto *ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			ai->steering_pos = nullptr;
		}
	}
}
