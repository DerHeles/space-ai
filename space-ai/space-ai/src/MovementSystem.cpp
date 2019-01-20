#include "MovementSystem.hpp"
#include "World.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "Configuration.hpp"

MovementSystem::MovementSystem(World *world)
	: System(world)
{
}

void MovementSystem::update(sf::Time elapsedTime)
{
	for (auto e : m_world->getEntityList())
	{
		if ((e->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM && (e->getComponentMask() & comp::MASK_VELOCITY) == comp::MASK_VELOCITY)
		{
			comp::Transform* pos = static_cast<comp::Transform*>(e->getAttachedComponents().find(comp::MASK_TRANSFORM)->second);
			comp::Velocity* velo = static_cast<comp::Velocity*>(e->getAttachedComponents().find(comp::MASK_VELOCITY)->second);

			if ((e->getComponentMask() & comp::MASK_ACCELERATION) == comp::MASK_ACCELERATION)
			{
				comp::Acceleration* accel = static_cast<comp::Acceleration*>(e->getAttachedComponents().find(comp::MASK_ACCELERATION)->second);
				velo->x += accel->x * elapsedTime.asSeconds();
				velo->y += accel->y * elapsedTime.asSeconds();

				if (abs(velo->x) > velo->x_max)
					velo->x = velo->x > 0 ? velo->x_max : -velo->x_max;
				if (abs(velo->y) > velo->y_max)
					velo->y = velo->y > 0 ? velo->y_max : -velo->y_max;

			}
			pos->x += velo->x * elapsedTime.asSeconds();
			if(pos->x <= 0)
				pos->x = cfg::GAME_AREA_WIDTH;
			else if(pos->x > cfg::GAME_AREA_WIDTH)
				pos->x = 0;

			pos->y += velo->y * elapsedTime.asSeconds();
			if (pos->y <= 0)
				pos->y = cfg::GAME_AREA_HEIGHT;
			else if (pos->y > cfg::GAME_AREA_HEIGHT)
				pos->y = 0;
			pos->rotation += velo->angular * elapsedTime.asSeconds();
			if(pos->rotation > 360)
			{
				pos->rotation -= 360.f;
			}
			else if(pos->rotation < 0)
			{
				pos->rotation += 360.f;
			}
			if ((e->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
			{
				comp::Target* target = static_cast<comp::Target*>(e->getAttachedComponents().find(comp::MASK_TARGET)->second);
				if(target->active)
				{
					target->line.setPoint1(sf::Vector2f(pos->x, pos->y));
				}
			}

			if ((e->getComponentMask() & comp::MASK_HEALTH) == comp::MASK_HEALTH)
			{
				comp::Health* health = static_cast<comp::Health*>(e->getAttachedComponents().find(comp::MASK_HEALTH)->second);
				health->bar.setPosition(pos->x, pos->y);
				health->bar.move(health->offset);
				health->backgroundBar.setPosition(pos->x, pos->y);
				health->backgroundBar.move(health->offset);
			}

			if ((e->getComponentMask() & comp::MASK_STORAGE) == comp::MASK_STORAGE)
			{
				comp::Storage* storage = static_cast<comp::Storage*>(e->getAttachedComponents().find(comp::MASK_STORAGE)->second);
				storage->bar.setPosition(pos->x, pos->y);
				storage->bar.move(storage->offset);
				storage->backgroundBar.setPosition(pos->x, pos->y);
				storage->backgroundBar.move(storage->offset);
			}
		}
	}
}
