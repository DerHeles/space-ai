#include "Pirate_FightPoliceState.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "VectorHelper.hpp"


namespace pirate
{
	FightPoliceState::FightPoliceState(StateMachine* pStateMachine, World* world, DeathSystem *deathSystem)
		:
		State(pStateMachine),
		m_world(world)
	{
		deathSystem->addReceiver(this);
	}

	void FightPoliceState::enter()
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

	void FightPoliceState::update(sf::Time elapsedTime)
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
		}
	}

	void FightPoliceState::exit()
	{
		m_seek.setTarget(nullptr);
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			ai->steering_pos = nullptr;
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));
			target->targetEntity = nullptr;
			target->active = false;
		}
	}

	void FightPoliceState::receiveEvent(Entity* entity)
	{
		if (m_seek.getTarget() == entity)
		{
			m_pStateMachine->changeToState("pirate_wander");
		}
	}
}
