#include "PoliceSystem.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include "World.hpp"
#include "Configuration.hpp"
#include <iostream>

PoliceSystem::PoliceSystem(World* world)
	:
	System(world),
	m_availablePoliceShips(cfg::MAX_COUNT_POLICE),
	m_policeStation(nullptr)
{
}

void PoliceSystem::update(sf::Time elapsedTime)
{
	//pending call list to avoid inserting in vector while iterating
	while (!m_pendingEmergencyCalls.empty())
	{
		Entity* e = m_world->createEntity("emergency");
		if (e != nullptr)
			if ((e->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
			{
				comp::Transform *trans = static_cast<comp::Transform*>(e->getComponent(comp::MASK_TRANSFORM));

				trans->x = m_pendingEmergencyCalls.front().x;
				trans->y = m_pendingEmergencyCalls.front().y;
			}
		m_pendingEmergencyCalls.pop_front();
	}

	std::vector<Entity*> answeredEmergencies;
	for (auto e : m_world->getEntityList())
	{
		if ((e->getComponentMask() & comp::MASK_EMERGENCY) == comp::MASK_EMERGENCY)
		{
			comp::Emergency *emergency = static_cast<comp::Emergency*>(e->getComponent(comp::MASK_EMERGENCY));
			emergency->time -= elapsedTime;
			if((emergency->time <= sf::Time::Zero && !emergency->callAnswered) || (emergency->callAnswered && emergency->shipArrived))
			{
				e->addComponent(new comp::Deletion);
			}
			else
			{
				if(m_availablePoliceShips > 0 && !emergency->callAnswered)
				{
					//send police ship
					--m_availablePoliceShips;
					answeredEmergencies.push_back(e);
					emergency->callAnswered = true;
					if ((m_policeStation->getComponentMask() & comp::MASK_STANDBY_SHIPS) == comp::MASK_STANDBY_SHIPS)
					{
						auto standby = static_cast<comp::StandbyShips*>(m_policeStation->getComponent(comp::MASK_STANDBY_SHIPS));
						standby->ships = std::max(--standby->ships, 0.f);
						standby->bar.setSize(sf::Vector2f(standby->ships / standby->maxShips * 180.f, 10.f));
					}
				}
			}
		}
	}
	for(auto e : answeredEmergencies)
	{
		Entity *police = m_world->createEntity("police");
		if ((police->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET
			&& (police->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
		{
			auto target = static_cast<comp::Target*>(police->getComponent(comp::MASK_TARGET));
			target->targetEntity = e;

			auto trans = static_cast<comp::Transform*>(police->getComponent(comp::MASK_TRANSFORM));
			trans->x = cfg::POSITION_POLICE_STATION.x;
			trans->y = cfg::POSITION_POLICE_STATION.y;
		}
	}
}

void PoliceSystem::makeEmergencyCall(float x, float y)
{
	m_pendingEmergencyCalls.push_back(sf::Vector2f(x, y));
}

void PoliceSystem::putShipOnStandby()
{
	++m_availablePoliceShips;
	if((m_policeStation->getComponentMask() & comp::MASK_STANDBY_SHIPS) == comp::MASK_STANDBY_SHIPS)
	{
		auto standby = static_cast<comp::StandbyShips*>(m_policeStation->getComponent(comp::MASK_STANDBY_SHIPS));
		standby->ships = std::min(++standby->ships, standby->maxShips);
		standby->bar.setSize(sf::Vector2f(standby->ships / standby->maxShips * 180.f, 10.f));
	}
}

void PoliceSystem::setPoliceStation(Entity* station)
{
	m_policeStation = station;
}
