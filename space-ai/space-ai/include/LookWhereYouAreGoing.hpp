#ifndef LOOK_WHERE_YOU_ARE_GOING_HPP
#define LOOK_WHERE_YOU_ARE_GOING_HPP

#include "Steering.hpp"

class Entity;

class LookWhereYouAreGoing : public AngularSteering
{
public:
	LookWhereYouAreGoing();
	void apply(Entity* pSelf) override;

private:
	float m_epsilon;
};
#endif