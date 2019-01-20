#ifndef STATE_HPP
#define STATE_HPP

#include <SFML/System/Time.hpp>

class StateMachine;

class State
{
public:
	State(StateMachine* pStateMachine);
	virtual ~State();

	virtual void enter() = 0;
	virtual void update(sf::Time elapsedTime) = 0;
	virtual void exit() = 0;
protected:
	StateMachine* m_pStateMachine;
};
#endif