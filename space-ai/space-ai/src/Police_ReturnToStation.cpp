#include "Police_ReturnToStation.hpp"
#include "World.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include <iostream>
#include "VectorHelper.hpp"

namespace police
{
	ReturnToStationState::ReturnToStationState(StateMachine* pStateMachine, World* world)
		:
		State(pStateMachine),
		m_world(world)
	{
	}

	void ReturnToStationState::enter()
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));

			Entity *station = nullptr;
			for(auto e : m_world->getEntityList())
			{
				if ((e->getComponentMask() & comp::MASK_GAMELOGIC) == comp::MASK_GAMELOGIC)
				{
					auto logic = static_cast<comp::GameLogic*>(e->getComponent(comp::MASK_GAMELOGIC));
					if(logic->role == "station_police")
					{
						station = e;
						target->targetEntity = e;
						break;
					}
				}
			}
			if(station != nullptr)
			{
				ai->steering_pos = &m_seek;
				m_seek.setTarget(target->targetEntity);
			}
		}
	}

	void ReturnToStationState::update(sf::Time elapsedTime)
	{
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto target = static_cast<comp::Target*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TARGET));
			if (target->targetEntity == nullptr)
			{
				std::cerr << "error returning to police station!" << std::endl;
				return;
			}
			if ((target->targetEntity->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
			{
				auto trans = static_cast<comp::Transform*>(target->targetEntity->getComponent(comp::MASK_TRANSFORM));
				if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
				{
					auto selfTrans = static_cast<comp::Transform*>(m_pStateMachine->getParent()->getComponent(comp::MASK_TRANSFORM));

					sf::Vector2f distanceVector = sf::Vector2f(selfTrans->x - trans->x, selfTrans->y - trans->y);
					if (vhelp::length(distanceVector) <= 50.f)
					{
						m_pStateMachine->getParent()->addComponent(new comp::Deletion);
					}
				}
			}
		}
	}

	void ReturnToStationState::exit()
	{
		m_seek.setTarget(nullptr);
		if ((m_pStateMachine->getParent()->getComponentMask() & comp::MASK_AI) == comp::MASK_AI && (m_pStateMachine->getParent()->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			auto ai = static_cast<comp::AI*>(m_pStateMachine->getParent()->getComponent(comp::MASK_AI));
			ai->steering_pos = nullptr;
		}
	}
}
