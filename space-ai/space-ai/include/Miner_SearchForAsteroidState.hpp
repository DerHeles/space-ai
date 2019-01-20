#ifndef MINER_SEARCH_FOR_ASTEROID_STATE_HPP
#define MINER_SEARCH_FOR_ASTEROID_STATE_HPP

#include "StateMachine.hpp"
#include "Wander.hpp"

class World;

namespace miner
{
	class SearchForAsteroidState : public State
	{
	public:
		SearchForAsteroidState(StateMachine *pStateMachine, World *world);

		void enter() override;
		void update(sf::Time elapsedTime) override;
		void exit() override;

	private:
		Wander m_wander;
		World *m_world;
	};
}

#endif