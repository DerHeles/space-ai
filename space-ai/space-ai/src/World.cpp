#include <chrono>
#include <iostream>
#include "World.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include "Configuration.hpp"
#include "LookWhereYouAreGoing.hpp"
#include "DeathSystem.hpp"
#include "PoliceSystem.hpp"

#include "Miner_SearchForAsteroidState.hpp"
#include "Miner_SeekAsteroidState.hpp"
#include "Miner_DismantleAsteroid.hpp"
#include "Miner_SeekStationState.hpp"
#include "Miner_FleeState.hpp"
#include "Miner_WorkState.hpp"
#include "Miner_RepairState.hpp"

#include "Pirate_FightPoliceState.hpp"
#include "Pirate_KillMinerState.hpp"
#include "Pirate_WanderState.hpp"

#include "Police_WanderState.hpp"
#include "Police_EmergencyState.hpp"
#include "Police_FightState.hpp"
#include "Police_ReturnToStation.hpp";
#include "Police_PatrolState.hpp"

World::World(unsigned int width, unsigned int height)
	:
	m_generator(std::chrono::system_clock::now().time_since_epoch().count()),
	m_generator2(std::chrono::system_clock::now().time_since_epoch().count() + 200),
	m_xPosDistribution(0 + cfg::SPAWN_BORDER, width - cfg::SPAWN_BORDER),
	m_yPosDistribution(0 + cfg::SPAWN_BORDER, height - cfg::SPAWN_BORDER),
	m_deathSystem(nullptr),
	m_policeSystem(nullptr),
	m_currentHealStation(0)
{
	m_randomX = std::bind(m_xPosDistribution, m_generator);
	m_randomY = std::bind(m_yPosDistribution, m_generator2);

	if (!m_texturePirate.loadFromFile("img/pirate.png") || !m_texturePolice.loadFromFile("img/police.png") || !m_textureAsteroid.loadFromFile("img/asteroid.png")
		|| !m_textureMiner.loadFromFile("img/miner.png") || !m_textureStationMiner.loadFromFile("img/station_miner.png")
		|| !m_textureStationHeal.loadFromFile("img/station_heal.png") || !m_textureStationPolice.loadFromFile("img/station_police.png")
		|| !m_textureEmergency.loadFromFile("img/emergency.png"))
		std::cerr << "World ctor error loading textures!" << std::endl;
}

void World::init()
{
	createEntity("station_miner");
	createEntity("station_heal");
	createEntity("station_heal");
	createEntity("station_heal");
	createEntity("station_heal");
	m_policeSystem->setPoliceStation(createEntity("station_police"));
}

const std::vector<Entity*>& World::getEntityList() const
{
	return m_entities;
}

void World::deleteDeadEntities(const std::vector<Entity*>& list)
{
	for(auto e : list)
	{
		for(auto it = m_entities.begin(); it != m_entities.end();)
		{
			if (e == *it)
			{
				it = m_entities.erase(it);
				break;
			}
			++it;
		}
	}
}

void World::setDeathSystem(DeathSystem* deathSystem)
{
	m_deathSystem = deathSystem;
}

void World::setPoliceSystem(PoliceSystem* policeSystem)
{
	m_policeSystem = policeSystem;
}

Entity* World::createEntity(const std::string& type)
{
	Entity *entity = nullptr;
	if (type == "asteroid")
	{
		entity = new Entity;
		comp::Render *render = new comp::Render;
		render->sprite.setTexture(m_textureAsteroid);
		render->sprite.setOrigin(50.f, 50.f);
		entity->addComponent(render);

		comp::Transform *pos = new comp::Transform;
		pos->x = m_randomX();
		pos->y = m_randomY();
		pos->rotation = 0;
		entity->addComponent(pos);

		comp::GameLogic *logic = new comp::GameLogic;
		logic->role = "asteroid";
		entity->addComponent(logic);

		comp::Health *health = new comp::Health;
		health->hp = 220.f;
		health->maxHP = 220.f;
		health->bar.setPosition(pos->x, pos->y);
		health->bar.move(cfg::HP_BAR_OFFSET_ASTEROID);
		health->bar.setFillColor(sf::Color::Green);
		health->bar.setSize(sf::Vector2f((health->hp / health->maxHP) * 100.f, 10.f));

		health->backgroundBar.setPosition(pos->x, pos->y);
		health->backgroundBar.move(cfg::HP_BAR_OFFSET_ASTEROID);
		health->backgroundBar.setFillColor(sf::Color::Red);
		health->backgroundBar.setSize(sf::Vector2f(100.f, 10.f));
		health->backgroundBar.setOutlineColor(sf::Color(200, 200, 200));
		health->backgroundBar.setOutlineThickness(2.f);
		health->offset = cfg::HP_BAR_OFFSET_ASTEROID;
		entity->addComponent(health);

		m_entities.push_back(entity);
	}
	else if (type == "station_miner")
	{
		entity = new Entity;
		comp::Render *render = new comp::Render;
		render->sprite.setTexture(m_textureStationMiner);
		render->sprite.setOrigin(95.f, 95.f);
		entity->addComponent(render);


		comp::Transform *pos = new comp::Transform;
		pos->x = cfg::POSITION_MINER_STATION.x;
		pos->y = cfg::POSITION_MINER_STATION.y;
		pos->rotation = 0;
		entity->addComponent(pos);

		comp::Velocity *velo = new comp::Velocity;
		velo->x = 0.f;
		velo->y = 0.f;
		velo->x_max = 0.f;
		velo->y_max = 0.f;
		velo->angular = 10.f;
		entity->addComponent(velo);

		comp::GameLogic *logic = new comp::GameLogic;
		logic->role = type;
		entity->addComponent(logic);

		m_entities.push_back(entity);
	}
	else if (type == "station_police")
	{
		entity = new Entity;
		comp::Render *render = new comp::Render;
		render->sprite.setTexture(m_textureStationPolice);
		render->sprite.setOrigin(100.f, 100.f);
		entity->addComponent(render);

		comp::Transform *pos = new comp::Transform;
		pos->x = cfg::POSITION_POLICE_STATION.x;
		pos->y = cfg::POSITION_POLICE_STATION.y;
		pos->rotation = 0;
		entity->addComponent(pos);

		comp::Velocity *velo = new comp::Velocity;
		velo->x = 0.f;
		velo->y = 0.f;
		velo->x_max = 0.f;
		velo->y_max = 0.f;
		velo->angular = 10.f;
		entity->addComponent(velo);

		comp::StandbyShips *standby = new comp::StandbyShips;
		standby->ships = cfg::MAX_COUNT_POLICE;
		standby->maxShips = cfg::MAX_COUNT_POLICE;
		standby->bar.setPosition(pos->x, pos->y);
		standby->bar.move(cfg::STANDBY_SHIPS_BAR_OFFSET);
		standby->bar.setFillColor(sf::Color(255, 255, 0));
		standby->bar.setSize(sf::Vector2f((standby->ships / standby->maxShips) * 180.f, 10.f));

		standby->backgroundBar.setPosition(pos->x, pos->y);
		standby->backgroundBar.move(cfg::STANDBY_SHIPS_BAR_OFFSET);
		standby->backgroundBar.setFillColor(sf::Color(128, 128, 0));
		standby->backgroundBar.setSize(sf::Vector2f(180.f, 10.f));
		standby->backgroundBar.setOutlineColor(sf::Color(200, 200, 200));
		standby->backgroundBar.setOutlineThickness(2.f);
		standby->offset = cfg::STANDBY_SHIPS_BAR_OFFSET;
		entity->addComponent(standby);

		comp::GameLogic *logic = new comp::GameLogic;
		logic->role = type;
		entity->addComponent(logic);

		m_entities.push_back(entity);
	}
	else if(type == "station_heal")
	{
		entity = new Entity;
		comp::Render *render = new comp::Render;
		render->sprite.setTexture(m_textureStationHeal);
		render->sprite.setOrigin(95.f, 95.f);
		entity->addComponent(render);

		comp::Transform *pos = new comp::Transform;
		if(m_currentHealStation == 0)
		{
			pos->x = 400.f;
			pos->y = 400.f;
			++m_currentHealStation;
		}
		else if (m_currentHealStation == 1)
		{
			pos->x = cfg::GAME_AREA_WIDTH - 400.f;
			pos->y = 400.f;
			++m_currentHealStation;
		}
		else if (m_currentHealStation == 2)
		{
			pos->x = cfg::GAME_AREA_WIDTH - 400.f;
			pos->y = cfg::GAME_AREA_HEIGHT - 400.f;
			++m_currentHealStation;
		}
		else
		{
			pos->x = 400.f;
			pos->y = cfg::GAME_AREA_HEIGHT - 400.f;
			++m_currentHealStation;
		}
		pos->rotation = 0;
		entity->addComponent(pos);

		comp::Velocity *velo = new comp::Velocity;
		velo->x = 0.f;
		velo->y = 0.f;
		velo->x_max = 0.f;
		velo->y_max = 0.f;
		velo->angular = -10.f;
		entity->addComponent(velo);

		comp::GameLogic *logic = new comp::GameLogic;
		logic->role = type;
		entity->addComponent(logic);

		m_entities.push_back(entity);
	}
	else if(type == "police")
	{
		entity = new Entity;

		comp::Render *render = new comp::Render;
		render->sprite.setTexture(m_texturePolice);
		render->sprite.setOrigin(47.f, 50.f);
		entity->addComponent(render);

		comp::Transform *pos = new comp::Transform;
		pos->x = m_randomX();
		pos->y = m_randomY();
		pos->rotation = 0.f;
		entity->addComponent(pos);

		comp::Velocity *velo = new comp::Velocity;
		velo->x = 0.f;
		velo->y = 0.f;
		velo->x_max = 220.f;
		velo->y_max = 220.f;
		velo->angular = 0.f;
		entity->addComponent(velo);

		comp::Target *target = new comp::Target;
		target->targetEntity = nullptr;
		target->line.setPoint1(sf::Vector2f(pos->x, pos->y));
		target->line.setThickness(6.f);
		target->line.setColor(sf::Color(0, 0, 255, 128));
		target->active = false;
		entity->addComponent(target);

		comp::Acceleration *accel = new comp::Acceleration;
		accel->x = 0.f;
		accel->y = 0.f;
		accel->max = 70.f;
		entity->addComponent(accel);

		comp::GameLogic *logic = new comp::GameLogic;
		logic->role = type;
		entity->addComponent(logic);

		comp::Health *health = new comp::Health;
		health->hp = 400.f;
		health->maxHP = 400.f;
		health->bar.setPosition(pos->x, pos->y);
		health->bar.move(cfg::HP_BAR_OFFSET_POLICE);
		health->bar.setFillColor(sf::Color::Green);
		health->bar.setSize(sf::Vector2f((health->hp / health->maxHP) * 100.f, 10.f));

		health->backgroundBar.setPosition(pos->x, pos->y);
		health->backgroundBar.move(cfg::HP_BAR_OFFSET_POLICE);
		health->backgroundBar.setFillColor(sf::Color::Red);
		health->backgroundBar.setSize(sf::Vector2f(100.f, 10.f));
		health->backgroundBar.setOutlineColor(sf::Color(200, 200, 200));
		health->backgroundBar.setOutlineThickness(2.f);
		health->offset = cfg::HP_BAR_OFFSET_POLICE;
		entity->addComponent(health);

		comp::Attack *attack = new comp::Attack;
		attack->power = 50.f;
		entity->addComponent(attack);

		comp::AI *ai = new comp::AI;
		ai->steering_ang = new LookWhereYouAreGoing();
		StateMachine *sm = new StateMachine(entity);
		sm->addState("police_emergency", new police::EmergencyState(sm, this));

		police::PatrolState* patrol = new police::PatrolState(sm, this);

		patrol->addState("police_wander", new police::WanderState(patrol, this, patrol));
		patrol->addState("police_fight", new police::FightState(patrol, this, m_deathSystem));

		sm->addState("police_patrol", patrol);
		sm->addState("police_returnToStation", new police::ReturnToStationState(sm, this));

		ai->stateMachine = sm;
		entity->addComponent(ai);

		m_entities.push_back(entity);
	}
	else if(type == "miner")
	{
		entity = new Entity;
		comp::Render *render = new comp::Render;
		render->sprite.setTexture(m_textureMiner);
		render->sprite.setOrigin(30.f, 51.f);
		entity->addComponent(render);

		comp::Transform *pos = new comp::Transform;
		pos->x = m_randomX();
		pos->y = m_randomY();
		pos->rotation = 0.f;
		entity->addComponent(pos);

		comp::Velocity *velo = new comp::Velocity;
		velo->x = 0.f;
		velo->y = 0.f;
		velo->x_max = 200.f;
		velo->y_max = 200.f;
		velo->angular = 0.f;
		entity->addComponent(velo);

		comp::Acceleration *accel = new comp::Acceleration;
		accel->x = 0;
		accel->y = 0;
		accel->max = 90.f;
		entity->addComponent(accel);

		comp::Target *target = new comp::Target;
		target->targetEntity = nullptr;
		target->line.setPoint1(sf::Vector2f(pos->x, pos->y));
		target->line.setThickness(6.f);
		target->line.setColor(sf::Color::Yellow);
		target->active = false;
		entity->addComponent(target);

		comp::GameLogic *logic = new comp::GameLogic;
		logic->role = type;
		entity->addComponent(logic);

		comp::Health *health = new comp::Health;
		health->hp = 120.f;
		health->maxHP = 120.f;
		health->bar.setPosition(pos->x, pos->y);
		health->bar.move(cfg::HP_BAR_OFFSET_MINER);
		health->bar.setFillColor(sf::Color::Green);
		health->bar.setSize(sf::Vector2f((health->hp / health->maxHP) * 100.f, 10.f));

		health->backgroundBar.setPosition(pos->x, pos->y);
		health->backgroundBar.move(cfg::HP_BAR_OFFSET_MINER);
		health->backgroundBar.setFillColor(sf::Color::Red);
		health->backgroundBar.setSize(sf::Vector2f(100.f, 10.f));
		health->backgroundBar.setOutlineColor(sf::Color(200, 200, 200));
		health->backgroundBar.setOutlineThickness(2.f);
		health->offset = cfg::HP_BAR_OFFSET_MINER;
		entity->addComponent(health);


		comp::Storage *storage = new comp::Storage;
		storage->material = 0.f;
		storage->maxMaterial = 200.f;
		storage->bar.setPosition(pos->x, pos->y);
		storage->bar.move(cfg::STORAGE_BAR_OFFSET_MINER);
		storage->bar.setFillColor(sf::Color(69, 247, 229));
		storage->bar.setSize(sf::Vector2f((storage->material / storage->maxMaterial) * 100.f, 10.f));

		storage->backgroundBar.setPosition(pos->x, pos->y);
		storage->backgroundBar.move(cfg::STORAGE_BAR_OFFSET_MINER);
		storage->backgroundBar.setFillColor(sf::Color(187, 58, 216));
		storage->backgroundBar.setSize(sf::Vector2f(100.f, 10.f));
		storage->backgroundBar.setOutlineColor(sf::Color(200, 200, 200));
		storage->backgroundBar.setOutlineThickness(2.f);
		storage->offset = cfg::STORAGE_BAR_OFFSET_MINER;
		entity->addComponent(storage);

		comp::Attack *attack = new comp::Attack;
		attack->power = 35.f;
		entity->addComponent(attack);

		comp::AI *ai = new comp::AI;
		ai->steering_ang = new LookWhereYouAreGoing();

		StateMachine *sm = new StateMachine(entity);
		auto ws = new miner::WorkState(sm, this);

		ws->addState("miner_searchForAsteroid", new miner::SearchForAsteroidState(ws, this));
		ws->addState("miner_seekAsteroid", new miner::SeekAsteroidState(ws, m_deathSystem));
		ws->addState("miner_dismantleAsteroid", new miner::DismantleAsteroidState(ws, m_deathSystem));
		ws->addState("miner_seekStation", new miner::SeekStationState(ws, this));

		sm->addState("miner_work", ws);
		sm->addState("miner_flee", new miner::FleeState(sm, this, m_policeSystem));
		sm->addState("miner_repair", new miner::RepairState(sm, this));

		ai->stateMachine = sm;
		entity->addComponent(ai);

		sm->changeToState("miner_searchForAsteroid");

		m_entities.push_back(entity);
	}
	else if (type == "pirate")
	{
		entity = new Entity;
		comp::Render *render = new comp::Render;
		render->sprite.setTexture(m_texturePirate);
		render->sprite.setOrigin(48.f, 50.f);
		entity->addComponent(render);

		comp::Transform *pos = new comp::Transform;
		pos->x = m_randomX();
		pos->y = m_randomY();
		pos->rotation = 0.f;
		entity->addComponent(pos);

		comp::Velocity *velo = new comp::Velocity;
		velo->x = 0.f;
		velo->y = 0.f;
		velo->x_max = 160.f;
		velo->y_max = 160.f;
		velo->angular = 0.f;
		entity->addComponent(velo);

		comp::Target *target = new comp::Target;
		target->targetEntity = nullptr;
		target->line.setPoint1(sf::Vector2f(pos->x, pos->y));
		target->line.setThickness(6.f);
		target->line.setColor(sf::Color(255, 0, 0, 128));
		target->active = false;
		entity->addComponent(target);


		comp::Acceleration *accel = new comp::Acceleration;
		accel->x = 0.f;
		accel->y = 0.f;
		accel->max = 50.f;
		entity->addComponent(accel);

		comp::GameLogic *logic = new comp::GameLogic;
		logic->role = "pirate";
		entity->addComponent(logic);

		comp::Health *health = new comp::Health;
		health->hp = 280.f;
		health->maxHP = 280.f;
		health->bar.setPosition(pos->x, pos->y);
		health->bar.move(cfg::HP_BAR_OFFSET_PIRATE);
		health->bar.setFillColor(sf::Color::Green);
		health->bar.setSize(sf::Vector2f((health->hp / health->maxHP) * 100.f, 10.f));

		health->backgroundBar.setPosition(pos->x, pos->y);
		health->backgroundBar.move(cfg::HP_BAR_OFFSET_PIRATE);
		health->backgroundBar.setFillColor(sf::Color::Red);
		health->backgroundBar.setSize(sf::Vector2f(100.f, 10.f));
		health->backgroundBar.setOutlineColor(sf::Color(200, 200, 200));
		health->backgroundBar.setOutlineThickness(2.f);

		health->offset = cfg::HP_BAR_OFFSET_PIRATE;
		entity->addComponent(health);

		comp::Attack *attack = new comp::Attack;
		attack->power = 15.f;
		entity->addComponent(attack);

		comp::AI *ai = new comp::AI;
		ai->steering_ang = new LookWhereYouAreGoing();

		StateMachine *sm = new StateMachine(entity);
		sm->addState("pirate_wander", new pirate::WanderState(sm, this));
		sm->addState("pirate_killMiner", new pirate::KillMinerState(sm, this, m_deathSystem));
		sm->addState("pirate_fightPolice", new pirate::FightPoliceState(sm, this, m_deathSystem));

		ai->stateMachine = sm;
		entity->addComponent(ai);

		m_entities.push_back(entity);
	}
	else if(type == "emergency")
	{
		entity = new Entity;
		comp::Render *render = new comp::Render;
		render->sprite.setTexture(m_textureEmergency);
		entity->addComponent(render);

		comp::Transform *pos = new comp::Transform;
		pos->x = 0.f;
		pos->y = 0.f;
		pos->rotation = 0.f;
		entity->addComponent(pos);

		comp::GameLogic *logic = new comp::GameLogic;
		logic->role = type;
		entity->addComponent(logic);

		comp::Emergency *emergency = new comp::Emergency;
		emergency->time = sf::seconds(5.f);
		entity->addComponent(emergency);

		m_entities.push_back(entity);
	}
	else
		std::cerr << "invalid entity type!" << std::endl;
	return entity;
}
