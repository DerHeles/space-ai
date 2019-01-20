#ifndef ATTACK_SYSTEM_HPP
#define ATTACK_SYSTEM_HPP

#include "System.hpp"

class AttackSystem : public System
{

public:
	explicit AttackSystem(World *world);

	void update(sf::Time elapsedTime) override;
};
#endif