#include "Police_FightState.hpp"
#include "World.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include "VectorHelper.hpp"

namespace police
{
	FightState::FightState(StateMachine* pStateMachine, World* world, DeathSystem *deathSystem)
		:
		State(pStateMachine),
		m_world(world)
	{
		deathSystem->addReceiver(this);
	}

	void FightState::enter()
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));
			if (target->targetEntity == nullptr)
			{
				m_pStateMachine->changeToState("police_wander");
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

	void FightState::update(sf::Time elapsedTime)
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));
			if (target->targetEntity == nullptr)
			{
				m_pStateMachine->changeToState("police_wander");
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
					if (vhelp::length(distanceVector) > 500.f)
					{
						m_pStateMachine->changeToState("police_wander");
					}
				}
			}
		}
	}

	void FightState::exit()
	{
		m_seek.setTarget(nullptr);
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			ai->steering_pos = nullptr;
		}
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));
			target->active = false;
			target->targetEntity = nullptr;
		}
	}

	void FightState::receiveEvent(Entity* entity)
	{
		if (m_seek.getTarget() == entity)
		{
			m_pStateMachine->changeToState("police_wander");
		}
	}
}
