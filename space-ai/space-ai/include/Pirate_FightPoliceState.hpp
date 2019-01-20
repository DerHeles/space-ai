#ifndef PIRATE_FIGHT_POLICE_STATE_HPP
#define PIRATE_FIGHT_POLICE_STATE_HPP

#include "StateMachine.hpp"
#include "Seek.hpp"
#include "DeathSystem.hpp"

class World;

namespace pirate
{
	class FightPoliceState : public State, public EventReceiver
	{
	public:
		FightPoliceState(StateMachine *pStateMachine, World *world, DeathSystem *deathSystem);

		void enter() override;
		void update(sf::Time elapsedTime) override;
		void exit() override;

		void receiveEvent(Entity* entity) override;

	private:
		World *m_world;
		Seek m_seek;
	};
}
#endif