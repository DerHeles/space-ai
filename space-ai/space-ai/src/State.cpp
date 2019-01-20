#include "State.hpp"
#include "StateMachine.hpp"

State::State(StateMachine* pStateMachine)
	:
	m_pStateMachine(pStateMachine)
{

}

State::~State()
{
}

