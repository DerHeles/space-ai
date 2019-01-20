#ifndef MINER_FLEE_STATE_HPP
#define MINER_FLEE_STATE_HPP

#include "StateMachine.hpp"
#include "Flee.hpp"

class World;
class PoliceSystem;

namespace miner
{
	class FleeState : public State
	{
	public:
		FleeState(StateMachine *pStateMachine, World *world, PoliceSystem *policeSystem);

		void enter() override;
		void update(sf::Time elapsedTime) override;
		void exit() override;

	private:
		Flee m_flee;
		World *m_world;
		PoliceSystem *m_policeSystem;
	};
}

#endif