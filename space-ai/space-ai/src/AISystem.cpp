#include "AISystem.hpp"
#include "World.hpp"
#include "Components.hpp"
#include "Entity.hpp"

AISystem::AISystem(World *world)
	:
	System(world)
{
}

void AISystem::update(sf::Time elapsedTime)
{
	for (auto e : m_world->getEntityList())
	{
		if ((e->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM
			&& (e->getComponentMask() & comp::MASK_VELOCITY) == comp::MASK_VELOCITY
			&& (e->getComponentMask() & comp::MASK_AI) == comp::MASK_AI)
		{
			comp::AI *ai = dynamic_cast<comp::AI*>(e->getComponent(comp::MASK_AI));
			if(ai)
			{
				if (ai->stateMachine != nullptr)
					ai->stateMachine->update(elapsedTime);
				if(ai->steering_pos != nullptr)
					ai->steering_pos->apply(e, elapsedTime);
				if (ai->steering_ang != nullptr)
					ai->steering_ang->apply(e);
			}
		}
	}
}
