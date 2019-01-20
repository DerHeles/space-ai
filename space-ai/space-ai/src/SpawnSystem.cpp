#include "SpawnSystem.hpp"
#include "DeathSystem.hpp"
#include "Configuration.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "World.hpp"

SpawnSystem::SpawnSystem(World* world, DeathSystem *deathSystem)
	:
	System(world),
	m_countMiner(0),
	m_countAsteroids(0),
	m_countPirates(0)
{
	deathSystem->addReceiver(this);
}

void SpawnSystem::receiveEvent(Entity* entity)
{
	if((entity->getComponentMask() & comp::MASK_GAMELOGIC) == comp::MASK_GAMELOGIC)
	{
		auto logic = static_cast<comp::GameLogic*>(entity->getComponent(comp::MASK_GAMELOGIC));
		if (logic->role == "miner")
			--m_countMiner;
		else if (logic->role == "asteroid")
			--m_countAsteroids;
		else if (logic->role == "pirate")
			--m_countPirates;
	}
}

void SpawnSystem::update(sf::Time elapsedTime)
{
	m_spawnTimer -= elapsedTime;
	if(m_spawnTimer <= sf::Time::Zero)
	{
		m_spawnTimer = cfg::SPAWN_INTERVAL;
		while (m_countMiner < cfg::MAX_COUNT_MINER)
		{
			m_world->createEntity("miner");
			++m_countMiner;
		}
		while (m_countAsteroids < cfg::MAX_COUNT_ASTEROIDS)
		{
			m_world->createEntity("asteroid");
			++m_countAsteroids;
		}
		while (m_countPirates < cfg::MAX_COUNT_PIRATES)
		{
			m_world->createEntity("pirate");
			++m_countPirates;
		}
	}
}
