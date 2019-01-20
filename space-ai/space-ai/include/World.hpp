#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <random>
#include <functional>
#include <SFML/Graphics/Texture.hpp>

class Entity;
class DeathSystem;
class PoliceSystem;

class World
{
public:
	World(unsigned int width, unsigned int height);
	void init();
	const std::vector<Entity*>& getEntityList() const;
	void deleteDeadEntities(const std::vector<Entity*>& list);
	void setDeathSystem(DeathSystem *deathSystem);
	void setPoliceSystem(PoliceSystem *policeSystem);
	Entity* createEntity(const std::string& type);

private:

	std::vector<Entity*> m_entities;
	std::mt19937 m_generator;
	std::mt19937 m_generator2;
	std::uniform_int_distribution<int> m_xPosDistribution;
	std::uniform_int_distribution<int> m_yPosDistribution;
	std::function<int()> m_randomX;
	std::function<int()> m_randomY;

	sf::Texture m_texturePirate;
	sf::Texture m_texturePolice;
	sf::Texture m_textureAsteroid;
	sf::Texture m_textureStation;
	sf::Texture m_textureMiner;
	sf::Texture m_textureStationMiner;
	sf::Texture m_textureStationPolice;
	sf::Texture m_textureStationHeal;
	sf::Texture m_textureEmergency;

	DeathSystem *m_deathSystem;
	PoliceSystem *m_policeSystem;

	short m_currentHealStation; //indicates spawn position
};
#endif