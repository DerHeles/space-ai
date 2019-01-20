#ifndef MINER_SEEK_ASTEROID_STATE_HPP
#define MINER_SEEK_ASTEROID_STATE_HPP

#include "StateMachine.hpp"
#include "Seek.hpp"
#include "EventReceiver.hpp"
#include "DeathSystem.hpp"

namespace miner
{
	class SeekAsteroidState : public State, public EventReceiver
	{
	public:
		SeekAsteroidState(StateMachine *pStateMachine, DeathSystem* deathSystem);

		void enter() override;
		void update(sf::Time elapsedTime) override;
		void exit() override;

		void receiveEvent(Entity* entity) override;
	private:
		Seek m_seek;
	};
}

#endif