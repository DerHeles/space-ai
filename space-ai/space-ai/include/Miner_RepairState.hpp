#ifndef MINER_REPAIR_STATE_HPP
#define MINER_REPAIR_STATE_HPP

#include "StateMachine.hpp"
#include "Seek.hpp"

class World;
class PoliceSystem;

namespace miner
{
	class RepairState : public State
	{
	public:
		RepairState(StateMachine *pStateMachine, World *world);

		void enter() override;
		void update(sf::Time elapsedTime) override;
		void exit() override;

	private:
		Seek m_seek;
		World *m_world;
	};
}

#endif