#ifndef DEATH_SYSTEM_HPP
#define DEATH_SYSTEM_HPP

#include "System.hpp"
#include <vector>
#include "EventReceiver.hpp"

class PoliceSystem;

class DeathSystem : public System
{

public:
	explicit DeathSystem(World *world, PoliceSystem *policeSystem);

	void update(sf::Time elapsedTime) override;
	void addReceiver(EventReceiver* er);

private:
	std::vector<EventReceiver*> m_eventReceivers;
	PoliceSystem *m_policeSystem;
};
#endif