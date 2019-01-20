#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics.hpp>
#include "Steering.hpp"
#include "StateMachine.hpp"
#include "Line.hpp"

namespace comp
{
	enum Mask
	{
		MASK_NONE = 0,
		MASK_TRANSFORM = 1 << 0,
		MASK_VELOCITY = 1 << 1,
		MASK_ACCELERATION = 1 << 2,
		MASK_RENDER = 1 << 3,
		MASK_AI = 1 << 4,
		MASK_TARGET = 1 << 5,
		MASK_GAMELOGIC = 1 << 6,
		MASK_HEALTH = 1 << 7,
		MASK_STORAGE = 1 << 8,
		MASK_EMERGENCY = 1 << 9,
		MASK_ATTACK = 1 << 10,
		MASK_DELETION = 1 << 11,
		MASK_STANDBY_SHIPS = 1 << 12
	};

	struct Component
	{
		Component(Mask mask = MASK_NONE)
			:
			active(true),
			mask(mask)
		{}
		virtual ~Component(){}
		bool active;
		Mask mask;
	};

	struct Transform : Component
	{
		Transform()
			:
			Component(MASK_TRANSFORM)
		{}
		float x;
		float y;
		float rotation;
	};

	struct Velocity : Component
	{
		Velocity()
			:
			Component(MASK_VELOCITY)
		{}
		float x;
		float y;
		float x_max;
		float y_max;
		float angular;
	};

	struct Acceleration : Component
	{
		Acceleration()
			:
			Component(MASK_ACCELERATION)
		{}
		float x;
		float y;
		float max;
		float angular;
		float angular_max;
	};

	struct Render : Component
	{
		Render()
			:
			Component(MASK_RENDER)
		{}
		sf::Sprite sprite;
	};

	struct AI : Component
	{
		AI()
			:
			Component(MASK_AI),
			stateMachine(nullptr),
			steering_pos(nullptr),
			steering_ang(nullptr)
		{}

		~AI()
		{
			delete stateMachine;
			delete steering_pos;
			delete steering_ang;
		}
		StateMachine* stateMachine;
		PositionSteering* steering_pos;
		AngularSteering* steering_ang;
	};

	struct GameLogic : Component
	{
		GameLogic()
			:Component(MASK_GAMELOGIC)
		{}

		std::string role;
	};

	struct Target : Component
	{
		Target()
			:Component(MASK_TARGET)
		{}

		Entity *targetEntity;
		Line line;
	};

	struct Health : Component
	{
		Health()
			:Component(MASK_HEALTH)
		{}

		sf::RectangleShape bar;
		sf::RectangleShape backgroundBar;
		float hp;
		float maxHP;
		sf::Vector2f offset;
	};


	struct Storage : Component
	{
		Storage()
			:Component(MASK_STORAGE)
		{}

		sf::RectangleShape bar;
		sf::RectangleShape backgroundBar;
		float material;
		float maxMaterial;
		sf::Vector2f offset;
	};

	struct Attack : Component
	{
		Attack()
			:Component(MASK_ATTACK)
		{}

		float power;
		float range;
	};

	struct Deletion : Component
	{
		Deletion()
			:Component(MASK_DELETION)
		{}
	};

	struct Emergency : Component
	{
		Emergency()
			:Component(MASK_EMERGENCY),
			callAnswered(false),
			shipArrived(false)
		{}

		sf::Time time;
		bool callAnswered;
		bool shipArrived;
	};

	struct StandbyShips : Component
	{
		StandbyShips()
			:Component(MASK_STANDBY_SHIPS)
		{}

		sf::RectangleShape bar;
		sf::RectangleShape backgroundBar;
		float ships;
		float maxShips;
		sf::Vector2f offset;
	};
}
#endif
