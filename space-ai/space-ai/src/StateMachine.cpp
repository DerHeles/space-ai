#include "StateMachine.hpp"
#include <iostream>

StateMachine::StateMachine(Entity* pParent, StateMachine* pStateMachine)
	:
	State(pStateMachine),
	m_pParent(pParent)
{
}

StateMachine::~StateMachine()
{
	for (auto it = m_states.begin(); it != m_states.end(); ++it)
		delete it->second;
}

void StateMachine::addState(std::string name, State* pState)
{
	m_states.insert(std::pair<std::string, State*>(name, pState));
	//first added state is default state
	if (m_defaultState.empty())
		m_defaultState = name;
}

void StateMachine::changeToState(std::string newState)
{
	//if existing, leave current state
	if (!m_currentState.empty())
	{
		m_states.at(m_currentState)->exit();
		m_currentState.clear();
	}
	//change to state directly
	if (m_states.find(newState) != m_states.end())
	{
		m_states.at(newState)->enter();
		m_currentState = newState;
	}
	//or delegate change unto statemachine at higher level
	else
	{
		if (m_pStateMachine != nullptr)
			m_pStateMachine->changeToState(newState);
	}
}

void StateMachine::enter()
{
	if (m_currentState.empty())
		changeToState(m_defaultState);
}

void StateMachine::update(sf::Time elapsedTime)
{
	if (m_currentState.empty())
		changeToState(m_defaultState);
	m_states.at(m_currentState)->update(elapsedTime);
}

void StateMachine::exit()
{
	if (!m_currentState.empty())
	{
		m_states.at(m_currentState)->exit();
		m_currentState.clear();
	}
}

Entity* StateMachine::getParent() const
{
	return m_pParent;
}
