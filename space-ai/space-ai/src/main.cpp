#include "Application.hpp"

int main()
{
	srand(time(NULL));
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;

	Application app(settings);
	app.run();

	return 0;
}