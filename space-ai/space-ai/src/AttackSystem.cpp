#include "AttackSystem.hpp"
#include "World.hpp"
#include "Components.hpp"
#include "Entity.hpp"

AttackSystem::AttackSystem(World *world)
	:
	System(world)
{
}

void AttackSystem::update(sf::Time elapsedTime)
{
	for (auto e : m_world->getEntityList())
	{
		if ((e->getComponentMask() & comp::MASK_TARGET) == comp::MASK_TARGET && (e->getComponentMask() & comp::MASK_ATTACK) == comp::MASK_ATTACK)
		{
			comp::Target *target = static_cast<comp::Target*>(e->getComponent(comp::MASK_TARGET));
			comp::Attack *attack = static_cast<comp::Attack*>(e->getComponent(comp::MASK_ATTACK));

			if(target->active && (target->targetEntity->getComponentMask() & comp::MASK_HEALTH) == comp::MASK_HEALTH)
			{
				comp::Health *health = static_cast<comp::Health*>(target->targetEntity->getComponent(comp::MASK_HEALTH));
				float damage = attack->power * elapsedTime.asSeconds();
				health->hp -= damage;
				if ((e->getComponentMask() & comp::MASK_STORAGE) == comp::MASK_STORAGE)
				{
					comp::Storage *storage = static_cast<comp::Storage*>(e->getComponent(comp::MASK_STORAGE));
					if (health->hp < 0)
					{
						damage -= abs(health->hp);
						health->hp = 0;
					}
					storage->material += damage;
					storage->material = std::min(storage->material, storage->maxMaterial);
					storage->bar.setSize(sf::Vector2f(storage->material / storage->maxMaterial * 100.f, 10.f));
				}
				else
				{
					health->hp = std::max(health->hp, 0.f);
				}
				if (health->hp <= 0)
				{
					target->active = false;
				}

				health->bar.setSize(sf::Vector2f(health->hp / health->maxHP * 100.f, 10.f));
			}
		}
	}
}
