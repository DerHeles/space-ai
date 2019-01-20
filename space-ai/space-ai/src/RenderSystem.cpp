#include "RenderSystem.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include "World.hpp"
#include "Configuration.hpp"

RenderSystem::RenderSystem(sf::RenderWindow* window, World* world)
	:
	System(world),
	m_window(window)
{
}

void RenderSystem::update(sf::Time elapsedTime)
{
	//do nothing
}

void RenderSystem::render(float timeProgressFactor) const
{
	for(auto e : m_world->getEntityList())
	{
		if((e->getComponentMask() & comp::MASK_RENDER) == comp::MASK_RENDER)
		{
			comp::Render* renderComp = static_cast<comp::Render*>(e->getAttachedComponents().find(comp::MASK_RENDER)->second);
			if ((e->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
			{
				comp::Transform *transComp = static_cast<comp::Transform*>(e->getComponent(comp::MASK_TRANSFORM));
				if(transComp)
				{
					renderComp->sprite.setPosition(transComp->x, transComp->y);
					renderComp->sprite.setRotation(transComp->rotation);
				}
			}
			if ((e->getComponentMask() & comp::MASK_VELOCITY) == comp::MASK_VELOCITY)
			{
				comp::Velocity* velComp = static_cast<comp::Velocity*>(e->getAttachedComponents().find(comp::MASK_VELOCITY)->second);
				renderComp->sprite.move(timeProgressFactor * cfg::TIME_PER_FRAME * velComp->x, timeProgressFactor * cfg::TIME_PER_FRAME * velComp->y);
			}
			m_window->draw(renderComp->sprite);
		}
		if ((e->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET)
		{
			comp::Target* target = static_cast<comp::Target*>(e->getAttachedComponents().find(comp::MASK_TARGET)->second);
			if (target->active)
				m_window->draw(target->line);
		}

		if ((e->getComponentMask() & comp::MASK_HEALTH) == comp::MASK_HEALTH)
		{
			comp::Health* health = static_cast<comp::Health*>(e->getAttachedComponents().find(comp::MASK_HEALTH)->second);
			m_window->draw(health->backgroundBar);
			m_window->draw(health->bar);
		}

		if ((e->getComponentMask() & comp::MASK_STORAGE) == comp::MASK_STORAGE)
		{
			comp::Storage* storage = static_cast<comp::Storage*>(e->getAttachedComponents().find(comp::MASK_STORAGE)->second);
			m_window->draw(storage->backgroundBar);
			m_window->draw(storage->bar);
		}

		if ((e->getComponentMask() & comp::MASK_STANDBY_SHIPS) == comp::MASK_STANDBY_SHIPS)
		{
			comp::StandbyShips* standby = static_cast<comp::StandbyShips*>(e->getAttachedComponents().find(comp::MASK_STANDBY_SHIPS)->second);
			m_window->draw(standby->backgroundBar);
			m_window->draw(standby->bar);
		}
	}
}
