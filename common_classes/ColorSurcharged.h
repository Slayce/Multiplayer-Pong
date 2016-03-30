#ifndef DEF_COLORSURCHARGED
#define DEF_COLORSURCHARGED

#include <SFML\Network\Packet.hpp>
#include <SFML\Graphics\Color.hpp>
#include <iostream>

class ColorSurcharged : public sf::Color
{
public:
	ColorSurcharged() :sf::Color(){ ; }
	ColorSurcharged(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a) :sf::Color(r, g, b, a) { ; }
	ColorSurcharged(sf::Color color) : sf::Color(color){ ; }

	friend sf::Packet& operator<< (sf::Packet& packet, const ColorSurcharged& color_surcharged)
	{
		return packet << color_surcharged.r << color_surcharged.g << color_surcharged.b << color_surcharged.a;
	}

	friend sf::Packet& operator>> (sf::Packet& packet, ColorSurcharged& color_surcharged)
	{
		return packet >> color_surcharged.r >> color_surcharged.g >> color_surcharged.b >> color_surcharged.a;
	}

private:
};

#endif