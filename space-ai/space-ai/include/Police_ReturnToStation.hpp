#ifndef POLICE_RETURN_TO_STATION_STATE_HPP
#define POLICE_RETURN_TO_STATION_STATE_HPP

#include "StateMachine.hpp"
#include "Seek.hpp"

class World;

namespace police
{
	class ReturnToStationState : public State
	{
	public:
		ReturnToStationState(StateMachine *pStateMachine, World *world);

		void enter() override;
		void update(sf::Time elapsedTime) override;
		void exit() override;

	private:
		Seek m_seek;
		World *m_world;
	};
}
#endif