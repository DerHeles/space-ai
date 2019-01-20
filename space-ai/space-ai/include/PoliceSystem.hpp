#ifndef POLICE_SYSTEM_HPP
#define POLICE_SYSTEM_HPP

#include "System.hpp"
#include <SFML/Graphics.hpp>
#include <deque>

class Entity;

class PoliceSystem : public System
{
public:
	PoliceSystem(World *world);

	void update(sf::Time elapsedTime) override;
	void makeEmergencyCall(float x, float y);
	void putShipOnStandby();
	void setPoliceStation(Entity *station);

private:
	int m_availablePoliceShips;
	std::deque<sf::Vector2f> m_pendingEmergencyCalls;
	Entity *m_policeStation;
};
#endif