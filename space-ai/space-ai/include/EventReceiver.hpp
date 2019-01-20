#ifndef EVENT_RECEIVER_HPP
#define EVENT_RECEIVER_HPP

class Entity;

class EventReceiver
{
public:
	virtual void receiveEvent(Entity *entity) = 0;
};

#endif