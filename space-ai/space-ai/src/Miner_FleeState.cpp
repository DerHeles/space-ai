#include "Miner_FleeState.hpp"
#include "Entity.hpp"
#include "VectorHelper.hpp"
#include "World.hpp"
#include "PoliceSystem.hpp"

namespace miner
{
	FleeState::FleeState(StateMachine* pStateMachine, World *world, PoliceSystem *policeSystem)
		:
		State(pStateMachine),
		m_world(world),
		m_policeSystem(policeSystem)
	{
	}

	void FleeState::enter()
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI
			&& (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			ai->steering_pos = &m_flee;

			auto trans = static_cast<comp::Transform*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TRANSFORM));
			sf::Vector2f position = sf::Vector2f(trans->x, trans->y);

			for(auto e : m_world->getEntityList())
			{
				if ((e->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM
					&& (e->getComponentMask() & comp::MASK_GAMELOGIC) == comp::MASK_GAMELOGIC)
				{
					auto enemyTrans = static_cast<comp::Transform*>(e->getComponent(comp::MASK_TRANSFORM));
					auto logic = static_cast<comp::GameLogic*>(e->getComponent(comp::MASK_GAMELOGIC));
					if(logic->role == "pirate")
					{
						sf::Vector2f enemyPos = sf::Vector2f(enemyTrans->x, enemyTrans->y);
						sf::Vector2f distance = enemyPos - position;
						if (vhelp::length(distance) < 400.f)
							m_flee.addEnemy(e);
					}

				}

			}
			m_policeSystem->makeEmergencyCall(trans->x + 20.f, trans->y - 20.f);
		}
	}

	void FleeState::update(sf::Time elapsedTime)
	{
		m_flee.removeAllEnemies();
		bool flee = false;
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI
			&& (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			ai->steering_pos = &m_flee;

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
							m_flee.addEnemy(e);
							flee = true;
						}
					}

				}
			}
		}
		if (!flee)
		{
			if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_HEALTH) == comp::MASK_HEALTH)
			{
				auto health = static_cast<comp::Health*>(m_pStateMachine->getParent()->getComponent(comp::MASK_HEALTH));
				if(health->hp < 80.f)
					m_pStateMachine->changeToState("miner_repair");
				else
					m_pStateMachine->changeToState("miner_work");
				return;
			}
			m_pStateMachine->changeToState("miner_work");
		}
	}

	void FleeState::exit()
	{
		m_flee.removeAllEnemies();
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			ai->steering_pos = nullptr;
		}
	}
}
