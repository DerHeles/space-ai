#ifndef PIRATE_WANDER_STATE_HPP
#define PIRATE_WANDER_STATE_HPP

#include "StateMachine.hpp"
#include "Wander.hpp"

class World;

namespace pirate
{
	class WanderState : public State
	{
	public:
		WanderState(StateMachine *pStateMachine, World *world);

		void enter() override;
		void update(sf::Time elapsedTime) override;
		void exit() override;

	private:
		Wander m_wander;
		World *m_world;
	};
}
#endif