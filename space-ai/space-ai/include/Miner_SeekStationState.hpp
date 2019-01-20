#ifndef MINER_SEEK_STATION_STATE_HPP
#define MINER_SEEK_STATION_STATE_HPP

#include "StateMachine.hpp"
#include "Seek.hpp"

class World;

namespace miner
{
	class SeekStationState : public State
	{
	public:
		SeekStationState(StateMachine *pStateMachine, World *world);

		void enter() override;
		void update(sf::Time elapsedTime) override;
		void exit() override;

	private:
		Seek m_seek;
		World *m_world;
	};
}

#endif