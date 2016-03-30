#ifndef DEF_VECTOR2FSURCHARGED
#define DEF_VECTOR2FSURCHARGED

#include <SFML\Network\Packet.hpp>
#include <SFML\System\Vector2.hpp>
#include <iostream>

class Vector2fSurcharged : public sf::Vector2f
{
public:
	Vector2fSurcharged() :sf::Vector2f(){ ; }
	Vector2fSurcharged(float x, float y) :sf::Vector2f(x, y) { ; }
	Vector2fSurcharged(sf::Vector2f coordonnees) : sf::Vector2f(coordonnees){ ; }

	friend sf::Packet& operator<< (sf::Packet& packet, const Vector2fSurcharged& vector2f_surcharged)
	{
		return packet << vector2f_surcharged.x << vector2f_surcharged.y;
	}

	friend sf::Packet& operator>> (sf::Packet& packet, Vector2fSurcharged& vector2f_surcharged)
	{
		return packet >> vector2f_surcharged.x >> vector2f_surcharged.y;
	}

private:
};

#endif