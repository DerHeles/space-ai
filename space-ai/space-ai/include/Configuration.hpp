#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

namespace cfg
{
	extern const float TIME_PER_FRAME;
	extern const int SPAWN_BORDER;

	extern const unsigned int WINDOW_WIDTH;
	extern const unsigned int WINDOW_HEIGHT;
	extern const std::string WINDOW_TITLE;

	extern const float CAMERA_MOVE_OFFSET;

	extern const float GAME_AREA_WIDTH;
	extern const float GAME_AREA_HEIGHT;

	extern const sf::Vector2f HP_BAR_OFFSET_MINER;
	extern const sf::Vector2f HP_BAR_OFFSET_PIRATE;
	extern const sf::Vector2f HP_BAR_OFFSET_POLICE;
	extern const sf::Vector2f HP_BAR_OFFSET_ASTEROID;
	extern const sf::Vector2f STORAGE_BAR_OFFSET_MINER;
	extern const sf::Vector2f STANDBY_SHIPS_BAR_OFFSET;

	extern const sf::Vector2f POSITION_POLICE_STATION;
	extern const sf::Vector2f POSITION_MINER_STATION;

	extern const int MAX_COUNT_MINER;
	extern const int MAX_COUNT_ASTEROIDS;
	extern const int MAX_COUNT_PIRATES;
	extern const int MAX_COUNT_POLICE;

	extern const sf::Time SPAWN_INTERVAL;
}

#endif