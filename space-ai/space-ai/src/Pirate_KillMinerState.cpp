#include "Pirate_KillMinerState.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "VectorHelper.hpp"
#include "World.hpp"

namespace pirate
{
	KillMinerState::KillMinerState(StateMachine* pStateMachine, World* world, DeathSystem *deathSystem)
		:
		State(pStateMachine),
		m_world(world)
	{
		deathSystem->addReceiver(this);
	}

	void KillMinerState::enter()
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));
			if (target->targetEntity == nullptr)
			{
				m_pStateMachine->changeToState("pirate_wander");
				return;
			}
			ai->steering_pos = &m_seek;
			m_seek.setTarget(target->targetEntity);
			target->active = true;
			if ((target->targetEntity->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
			{
				auto trans = static_cast<comp::Transform*>(target->targetEntity->getComponent(comp::MASK_TRANSFORM));
				target->line.setPoint2(sf::Vector2f(trans->x, trans->y));
			}
		}
	}

	void KillMinerState::update(sf::Time elapsedTime)
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));
			if (target->targetEntity == nullptr)
			{
				m_pStateMachine->changeToState("pirate_wander");
				return;
			}
			if ((target->targetEntity->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
			{
				auto trans = static_cast<comp::Transform*>(target->targetEntity->getComponent(comp::MASK_TRANSFORM));
				target->line.setPoint2(sf::Vector2f(trans->x, trans->y));
				if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
				{
					auto selfTrans = static_cast<comp::Transform*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TRANSFORM));

					sf::Vector2f distanceVector = sf::Vector2f(selfTrans->x - trans->x, selfTrans->y - trans->y);
					if (vhelp::length(distanceVector) > 400.f)
					{
						target->active = false;
						target->targetEntity = nullptr;
						m_pStateMachine->changeToState("pirate_wander");
					}
				}
			}

			if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
			{
				comp::Transform *selfTrans = static_cast<comp::Transform*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TRANSFORM));
				comp::Target *selfTarget = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));

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
				}
			}
		}
	}

	void KillMinerState::exit()
	{
		m_seek.setTarget(nullptr);
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			ai->steering_pos = nullptr;
		}
	}

	void KillMinerState::receiveEvent(Entity* entity)
	{
		if (m_seek.getTarget() == entity)
		{
			m_pStateMachine->changeToState("pirate_wander");
		}
	}
}
