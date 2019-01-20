#include "Miner_RepairState.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "Seek.hpp"
#include "VectorHelper.hpp"
#include "World.hpp"

namespace miner
{
	RepairState::RepairState(StateMachine* pStateMachine, World* world)
		:
		State(pStateMachine),
		m_world(world)
	{
	}

	void RepairState::enter()
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));
			target->targetEntity = nullptr;
			target->active = false;

			float distance = std::numeric_limits<float>::max();
			for(auto e : m_world->getEntityList())
			{
				if ((e->getComponentMask() & comp::MASK_GAMELOGIC) == comp::MASK_GAMELOGIC
					&& (e->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM
					&& (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
				{
					auto logic = static_cast<comp::GameLogic*>(e->getComponent(comp::MASK_GAMELOGIC));
					auto selfTrans = static_cast<comp::Transform*>(e->getComponent(comp::MASK_TRANSFORM));
					auto trans = static_cast<comp::Transform*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TRANSFORM));

					sf::Vector2f pos = sf::Vector2f(selfTrans->x, selfTrans->y);
					sf::Vector2f enemyPos = sf::Vector2f(trans->x, trans->y);

					if(logic->role == "station_heal" && vhelp::length(pos - enemyPos) < distance)
					{
						distance = vhelp::length(pos - enemyPos);
						target->targetEntity = e;
					}
				}
			}
			if (target->targetEntity == nullptr)
			{
				m_pStateMachine->changeToState("miner_work");
				return;
			}
			ai->steering_pos = &m_seek;
			m_seek.setTarget(target->targetEntity);
		}
	}

	void RepairState::update(sf::Time elapsedTime)
	{
		//flee if pirate nearby
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
							m_pStateMachine->changeToState("miner_flee");
							return;
						}
					}

				}
			}
			if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
			{
				auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));
				if (target->targetEntity != nullptr && (target->targetEntity->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
				{
					auto targetTrans = static_cast<comp::Transform*>(target->targetEntity->getComponent(comp::MASK_TRANSFORM));

					sf::Vector2f distanceVector = sf::Vector2f(trans->x - targetTrans->x, trans->y - targetTrans->y);
					if (vhelp::length(distanceVector) <= 20.f) //station nearby
					{
						if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_HEALTH) == comp::MASK_HEALTH)
						{
							auto health = static_cast<comp::Health*>(m_pStateMachine->getParent()->getComponent(comp::MASK_HEALTH));
							health->hp = health->maxHP;
							health->bar.setSize(sf::Vector2f(100.f, 10.f));
						}
						m_pStateMachine->changeToState("miner_work");
					}
				}
			}
		}
	}

	void RepairState::exit()
	{
		m_seek.setTarget(nullptr);
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			ai->steering_pos = nullptr;
		}
	}
}
