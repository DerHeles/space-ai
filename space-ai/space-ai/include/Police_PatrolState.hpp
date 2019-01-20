#ifndef POLICE_PATROL_STATE_HPP
#define POLICE_PATROL_STATE_HPP

#include "StateMachine.hpp"

class World;

namespace police
{
	class PatrolState : public StateMachine
	{
	public:
		PatrolState(StateMachine *pStateMachine, World *world);

		void enter() override;
		void update(sf::Time elapsedTime) override;
		void exit() override;

		bool patrolTimeExpired() const;

	private:
		World *m_world;
		sf::Time m_patrolTime;
	};
}

#endif