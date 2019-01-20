#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <SFML/System/Time.hpp>
#include "State.hpp"
#include <map>

class Entity;

class StateMachine : public State
{
public:
	explicit StateMachine(Entity* pParent, StateMachine *pStateMachine = nullptr);
	~StateMachine();

	virtual void addState(std::string name, State *pState);
	virtual void changeToState(std::string newState);

	void enter() override;
	void update(sf::Time elapsedTime) override;
	void exit() override;

	Entity* getParent() const;

protected:
	std::map<std::string, State*> m_states;
	std::string m_currentState;
	std::string m_defaultState;
	Entity* m_pParent;
};

#endif