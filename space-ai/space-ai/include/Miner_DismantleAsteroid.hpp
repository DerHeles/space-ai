#ifndef MINER_DISMANTLE_ASTEROID_STATE_HPP
#define MINER_DISMANTLE_ASTEROID_STATE_HPP

#include "StateMachine.hpp"
#include "EventReceiver.hpp"
#include "DeathSystem.hpp"

namespace miner
{
	class DismantleAsteroidState : public State, public EventReceiver
	{
	public:
		DismantleAsteroidState(StateMachine *pStateMachine, DeathSystem* deathSystem);

		void enter() override;
		void update(sf::Time elapsedTime) override;
		void exit() override;

		void receiveEvent(Entity* entity) override;
	};
}

#endif