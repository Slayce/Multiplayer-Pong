#ifndef DEF_BALL
#define DEF_BALL

#include <SFML\Graphics.hpp>
#include "Vector2fSurcharged.h"

class Ball : public sf::CircleShape
{
public:
	Ball();
	Ball(float radius, Vector2fSurcharged direction, float speed = 2);
	float get_speed() const { return _speed; }

	enum Side { TOP, RIGHT, BOT, LEFT };

	void move_ball();
	void bounce(Side side);
	float get_radius() { return _radius; }
	Vector2fSurcharged const get_position() { return _position; }
	void set_position();
	void set_position(float x, float y);
	void set_speed(float additional_speed) { _speed = additional_speed; }

	friend sf::Packet& operator<< (sf::Packet& packet, const Ball& ball)
	{
		return packet << ball.getRadius() << ball._direction << ball.get_speed() << ball._position;
	}

	friend sf::Packet& operator>> (sf::Packet& packet, Ball& ball)
	{
		return packet >> ball._radius >> ball._direction >> ball._speed >> ball._position;
	}

private:
	float _radius;
	float _speed;
	Vector2fSurcharged _old_direction;
	Vector2fSurcharged _direction;
	Vector2fSurcharged _movement;
	Vector2fSurcharged _position;
};

#endif