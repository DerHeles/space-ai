#ifndef POLICE_FIGHT_STATE_HPP
#define POLICE_FIGHT_STATE_HPP

#include "StateMachine.hpp"
#include "Seek.hpp"
#include "DeathSystem.hpp"

class World;

namespace police
{
	class FightState : public State, public EventReceiver
	{
	public:
		FightState(StateMachine *pStateMachine, World *world, DeathSystem *deathSystem);

		void enter() override;
		void update(sf::Time elapsedTime) override;
		void exit() override;

		void receiveEvent(Entity* entity) override;

	private:
		Seek m_seek;
		World *m_world;
	};
}
#endif