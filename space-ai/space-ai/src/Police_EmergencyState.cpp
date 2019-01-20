#include "Police_EmergencyState.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "Seek.hpp"
#include "VectorHelper.hpp"
#include <iostream>

namespace police
{
	EmergencyState::EmergencyState(StateMachine* pStateMachine, World* world)
		:
		State(pStateMachine),
		m_world(world)
	{
	}

	void EmergencyState::enter()
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));
			if (target->targetEntity == nullptr)
			{
				std::cerr << "enter EmergencyState invalid target!" << std::endl;
				return;
			}
			ai->steering_pos = &m_seek;
			m_seek.setTarget(target->targetEntity);
		}
	}

	void EmergencyState::update(sf::Time elapsedTime)
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto selfTrans = static_cast<comp::Transform*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TRANSFORM));
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));

			if ((target->targetEntity != nullptr && target->targetEntity->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
			{
				auto targetTrans = static_cast<comp::Transform*>(target->targetEntity->getComponent(comp::MASK_TRANSFORM));

				sf::Vector2f distanceVector = sf::Vector2f(selfTrans->x - targetTrans->x, selfTrans->y - targetTrans->y);
				if (vhelp::length(distanceVector) <= 100.f) //emergency call nearby
				{
					if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_VELOCITY) == comp::MASK_VELOCITY)
					{
						auto velo = static_cast<comp::Velocity*>(m_pStateMachine->getParent()->getComponent(comp::MASK_VELOCITY));
						velo->x = 0.f;
						velo->y = 0.f;
						velo->angular = 0.f;
					}
					if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_ACCELERATION) == comp::MASK_ACCELERATION)
					{
						auto accel = static_cast<comp::Acceleration*>(m_pStateMachine->getParent()->getComponent(comp::MASK_ACCELERATION));
						accel->x = 0.f;
						accel->y = 0.f;
					}
					m_pStateMachine->changeToState("police_patrol");
					if ((target->targetEntity->getComponentMask() & comp::MASK_EMERGENCY) == comp::MASK_EMERGENCY)
					{
						auto emergency = static_cast<comp::Emergency*>(target->targetEntity->getComponent(comp::MASK_EMERGENCY));
						emergency->shipArrived = true;
					}
				}
			}
		}
	}

	void EmergencyState::exit()
	{
		m_seek.setTarget(nullptr);
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			ai->steering_pos = nullptr;
		}
	}
}
