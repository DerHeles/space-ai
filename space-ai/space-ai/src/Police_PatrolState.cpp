#include "Police_PatrolState.hpp"

police::PatrolState::PatrolState(StateMachine* pStateMachine, World* world)
	:
	StateMachine(pStateMachine->getParent(), pStateMachine),
	m_world(world)
{
}

void police::PatrolState::enter()
{
	m_patrolTime = sf::seconds(15.f);
	if (m_currentState.empty())
		changeToState(m_defaultState);
	else
		m_states.at(m_currentState)->enter();
}

void police::PatrolState::update(sf::Time elapsedTime)
{
	m_patrolTime -= elapsedTime;
	if (m_patrolTime <= sf::Time::Zero)
		m_patrolTime = sf::Time::Zero;

	StateMachine::update(elapsedTime);
}

void police::PatrolState::exit()
{
}

bool police::PatrolState::patrolTimeExpired() const
{
	return m_patrolTime == sf::Time::Zero;
}
