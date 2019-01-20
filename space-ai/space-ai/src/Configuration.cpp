#include "Configuration.hpp"

namespace cfg
{
	const float TIME_PER_FRAME = 1.f / 30.f;
	const int SPAWN_BORDER = 50;

	const unsigned int WINDOW_WIDTH = 1600;
	const unsigned int WINDOW_HEIGHT = 900;

	const std::string WINDOW_TITLE = "Space AI (c) Tobias Heiles";


	const float GAME_AREA_WIDTH = 6400.f;
	const float GAME_AREA_HEIGHT = 3600.f;

	const float CAMERA_MOVE_OFFSET = 60.f;

	const sf::Vector2f HP_BAR_OFFSET_MINER = sf::Vector2f(-45.f, 90.f);
	const sf::Vector2f HP_BAR_OFFSET_PIRATE = sf::Vector2f(-50.f, 65.f);
	const sf::Vector2f HP_BAR_OFFSET_POLICE = sf::Vector2f(-45.f, 70.f);
	const sf::Vector2f HP_BAR_OFFSET_ASTEROID = sf::Vector2f(-52.f, 60.f);
	const sf::Vector2f STORAGE_BAR_OFFSET_MINER = sf::Vector2f(-45.f, 120.f);
	const sf::Vector2f STANDBY_SHIPS_BAR_OFFSET = sf::Vector2f(-90.f, - 140.f);

	const sf::Vector2f POSITION_POLICE_STATION = sf::Vector2f(GAME_AREA_WIDTH / 2.f - 600.f, GAME_AREA_HEIGHT / 2.f);
	const sf::Vector2f POSITION_MINER_STATION = sf::Vector2f(GAME_AREA_WIDTH / 2.f + 600.f, GAME_AREA_HEIGHT / 2.f);

	const int MAX_COUNT_MINER = 12;
	const int MAX_COUNT_ASTEROIDS = 20;
	const int MAX_COUNT_PIRATES = 6;
	const int MAX_COUNT_POLICE = 8;

	const sf::Time SPAWN_INTERVAL = sf::seconds(25.f);
}