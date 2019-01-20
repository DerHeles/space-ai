#ifndef POLICE_WANDER_STATE_HPP
#define POLICE_WANDER_STATE_HPP

#include "StateMachine.hpp"
#include "Police_PatrolState.hpp"
#include "Wander.hpp"

class World;

namespace police
{
	class WanderState : public State
	{
	public:
		WanderState(StateMachine *pStateMachine, World *world, PatrolState* const patrolState);

		void enter() override;
		void update(sf::Time elapsedTime) override;
		void exit() override;

	private:
		Wander m_wander;
		World *m_world;
		PatrolState* const m_patrolState;
	};
}
#endif