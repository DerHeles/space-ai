#include "DeathSystem.hpp"
#include "World.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "PoliceSystem.hpp"

DeathSystem::DeathSystem(World* world, PoliceSystem *policeSystem)
	:
	System(world),
	m_policeSystem(policeSystem)
{
}

void DeathSystem::update(sf::Time elapsedTime)
{
	std::vector<Entity*> deadEntitys;
	for (auto e : m_world->getEntityList())
	{
		bool entityIsDead = false;
		if ((e->getComponentMask() & comp::MASK_HEALTH) == comp::MASK_HEALTH)
		{
			comp::Health *health = static_cast<comp::Health*>(e->getComponent(comp::MASK_HEALTH));
			if (health->hp <= 0)
			{
				entityIsDead = true;
			}
		}
		if ((e->getComponentMask() & comp::MASK_DELETION) == comp::MASK_DELETION)
		{
			entityIsDead = true;
		}
		if(entityIsDead)
		{
			for (auto rcv : m_eventReceivers)
				rcv->receiveEvent(e);
			deadEntitys.push_back(e);
			if ((e->getComponentMask() & comp::MASK_GAMELOGIC) == comp::MASK_GAMELOGIC)
			{
				auto logic = static_cast<comp::GameLogic*>(e->getComponent(comp::MASK_GAMELOGIC));
				if (logic->role == "police")
					m_policeSystem->putShipOnStandby();
			}
		}
	}
	if (!deadEntitys.empty())
		m_world->deleteDeadEntities(deadEntitys);
}

void DeathSystem::addReceiver(EventReceiver* er)
{
	m_eventReceivers.push_back(er);
}
