#ifndef SPAWN_SYSTEM_HPP
#define SPAWN_SYSTEM_HPP

#include "System.hpp"
#include "EventReceiver.hpp"

class DeathSystem;
class PoliceSystem;

class SpawnSystem : public System, public EventReceiver
{
public:
	explicit SpawnSystem(World *world, DeathSystem *deathSystem);

	void update(sf::Time elapsedTime) override;
	void receiveEvent(Entity* entity) override;

private:
	sf::Time m_spawnTimer;

	int m_countMiner;
	int m_countAsteroids;
	int m_countPirates;
};
#endif