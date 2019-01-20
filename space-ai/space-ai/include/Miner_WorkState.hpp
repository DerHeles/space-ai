#ifndef MINER_WORK_STATE_HPP
#define MINER_WORK_STATE_HPP

#include "StateMachine.hpp"

class World;

namespace miner
{
	class WorkState : public StateMachine
	{
	public:
		WorkState(StateMachine *pStateMachine, World *world);

		void enter() override;
		void update(sf::Time elapsedTime) override;
		void exit() override;

	private:
		World *m_world;
	};
}

#endif