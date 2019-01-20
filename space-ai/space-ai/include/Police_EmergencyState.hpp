#ifndef POLICE_EMERGENCY_STATE_HPP
#define POLICE_EMERGENCY_STATE_HPP

#include "StateMachine.hpp"
#include "Seek.hpp"

class World;

namespace police
{
	class EmergencyState : public State
	{
	public:
		EmergencyState(StateMachine *pStateMachine, World *world);

		void enter() override;
		void update(sf::Time elapsedTime) override;
		void exit() override;

	private:
		Seek m_seek;
		World *m_world;
	};
}
#endif