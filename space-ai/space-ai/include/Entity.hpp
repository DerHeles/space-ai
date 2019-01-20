#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <map>
#include "Components.hpp"

class Entity
{
public:
	Entity();
	~Entity();

	void addComponent(comp::Component *component);
	const std::map<int, comp::Component*>& getAttachedComponents() const;
	int getComponentMask() const;
	comp::Component* getComponent(int componentMask) const;

private:
	std::map<int, comp::Component*> m_components;
	int m_componentMask; //indicates the attached components
};
#endif