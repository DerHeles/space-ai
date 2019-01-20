#include "LookWhereYouAreGoing.hpp"
#include "Components.hpp"
#include "VectorHelper.hpp"
#include "Entity.hpp"

LookWhereYouAreGoing::LookWhereYouAreGoing()
	:
	m_epsilon(0.000001f)
{

}

void LookWhereYouAreGoing::apply(Entity* pSelf)
{
	if ((pSelf->getComponentMask() & comp::MASK_VELOCITY) == comp::MASK_VELOCITY
		&& (pSelf->getComponentMask() & comp::MASK_ACCELERATION) == comp::MASK_ACCELERATION
		&& (pSelf->getComponentMask() & comp::MASK_TRANSFORM) == comp::MASK_TRANSFORM)
	{
		auto velo = static_cast<comp::Velocity*>(pSelf->getComponent(comp::MASK_VELOCITY));
		auto trans = static_cast<comp::Transform*>(pSelf->getComponent(comp::MASK_TRANSFORM));
		auto accel = static_cast<comp::Acceleration*>(pSelf->getComponent(comp::MASK_ACCELERATION));

		if (vhelp::length(sf::Vector2f(velo->x, velo->y)) < m_epsilon)
		{
			accel->angular = 0.f;
			return;
		}

		//rotation angle depending on the velocity
		float angle = (180.f / 3.1415927f) * std::atan2(velo->y, velo->x);
		trans->rotation = angle;
		velo->angular = 0.f;
	}

}
