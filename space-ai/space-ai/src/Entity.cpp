#include "Entity.hpp"
#include "Components.hpp"
#include <iostream>

Entity::Entity()
	:
	m_componentMask(0)
{
}

Entity::~Entity()
{
	for (auto it = m_components.begin(); it != m_components.end(); ++it)
		delete it->second;
}

void Entity::addComponent(comp::Component *component)
{
	if(m_components.find(component->mask) == m_components.end())
	{
		m_components.insert(std::pair<int, comp::Component*>(component->mask, component));
		m_componentMask |= component->mask;
	}
	else
	{
		std::cerr << "addComponent: component already attached!" << std::endl;
		delete component;
	}
}

const std::map<int, comp::Component*>& Entity::getAttachedComponents() const
{
	return m_components;
}

int Entity::getComponentMask() const
{
	return m_componentMask;
}


comp::Component* Entity::getComponent(int componentMask) const
{
	if((m_componentMask & componentMask) == componentMask)
		return m_components.find(componentMask)->second;
	std::cerr << "entity has no comp " << componentMask << std::endl;
	return nullptr;
}
