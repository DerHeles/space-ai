#ifndef FLEE_HPP
#define FLEE_HPP

#include "Steering.hpp"
#include <vector>

class Flee : public PositionSteering
{
public:
	Flee();
	void addEnemy(Entity *enemy);
	bool removeEnemy(Entity *enemy); //true if last enemy was deleted
	void removeAllEnemies();
	void apply(Entity* pSelf, sf::Time elapsedTime) override;

private:
	std::vector<Entity*> m_enemies;

};
#endif