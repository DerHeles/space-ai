#ifndef AI_SYSTEM_HPP
#define AI_SYSTEM_HPP

#include "System.hpp"

class AISystem : public System
{

public:
	explicit AISystem(World *world);

	void update(sf::Time elapsedTime) override;
};
#endif