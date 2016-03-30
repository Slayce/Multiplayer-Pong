#ifndef DEF_RACKET
#define DEF_RACKET

#include <SFML\Graphics.hpp>
#include "Vector2fSurcharged.h"
#include "ColorSurcharged.h"

class Racket : public sf::RectangleShape
{
public:
	Racket();
	Racket(Vector2fSurcharged dimensions, float speed = 3);
	float get_speed() const { return _speed; }
	void move_right(float window_width);
	void move_left();
	Vector2fSurcharged get_dimensions() { return _dimensions; }
	ColorSurcharged get_color() { return _color; }
	void set_position();
	void set_position(float x, float y);
	void set_dimensions(Vector2fSurcharged dimensions) { _dimensions = dimensions; }
	void set_color(ColorSurcharged color) { _color = color; }

	friend sf::Packet& operator<< (sf::Packet& packet, const Racket& racket)
	{
		return packet << racket._speed << racket._position << racket._dimensions << racket._color;
	}

	friend sf::Packet& operator>> (sf::Packet& packet, Racket& racket)
	{
		return packet >> racket._speed >> racket._position >> racket._dimensions >> racket._color;
	}

private:
	float _speed;
	Vector2fSurcharged _dimensions;
	Vector2fSurcharged _position;
	ColorSurcharged _color;
};

#endif