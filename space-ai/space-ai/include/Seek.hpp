#ifndef SEEK_HPP
#define SEEK_HPP

#include "Steering.hpp"

class Seek : public PositionSteering
{
public:
	Seek();
	Seek(Entity *pTarget);
	Entity* const getTarget() const;
	void setTarget(Entity *pTarget);
	void apply(Entity* pSelf, sf::Time elapsedTime) override;

private:

	Entity *m_pTarget;

};
#endif