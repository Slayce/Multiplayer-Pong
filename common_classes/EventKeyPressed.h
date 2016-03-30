#ifndef DEF_EVENTKEYPRESSED
#define DEF_EVENTKEYPRESSED

#include <SFML\Network\Packet.hpp>
#include <SFML\Window\Event.hpp>

class EventKeyPressed : public sf::Event
{
public:
	EventKeyPressed() : sf::Event() { this->type = sf::Event::EventType::KeyPressed; }

	friend sf::Packet& operator<< (sf::Packet& packet, const EventKeyPressed& event_key_pressed)
	{
		return packet << static_cast<int>(event_key_pressed.key.code);
	}

private:
};

#endif