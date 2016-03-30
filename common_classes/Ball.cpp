#include "Ball.h"

Ball::Ball() : sf::CircleShape(0),
_radius(0),
_speed(0),
_direction(0, 0)
{}

Ball::Ball(float radius, Vector2fSurcharged direction, float speed) : sf::CircleShape(radius),
_radius(radius),
_direction(direction.x, direction.y),
_movement(direction.x + speed, direction.y + speed),
_speed(speed)
{}

void Ball::move_ball()
{
	Vector2fSurcharged new_position(this->getPosition().x + _movement.x, this->getPosition().y + _movement.y);
	this->set_position(new_position.x, new_position.y);
}

void Ball::bounce(Side side)
{
	_old_direction = _direction;

	switch (side)
	{
	case Ball::TOP:
		this->_direction = Vector2fSurcharged(_old_direction.x, -(_old_direction.y));
		break;
	case Ball::RIGHT:
		this->_direction = Vector2fSurcharged(-(_old_direction.x), _old_direction.y);
		break;
	case Ball::BOT:
		this->_direction = Vector2fSurcharged(_old_direction.x, -(_old_direction.y));
		break;
	case Ball::LEFT:
		this->_direction = Vector2fSurcharged(-(_old_direction.x), _old_direction.y);
		break;
	}

	if (_direction.x < 0)
	{
		this->_movement.x = _direction.x - _speed;
	}
	else
	{
		this->_movement.x = _direction.x + _speed;
	}
	if (_direction.y < 0)
	{
		this->_movement.y = _direction.y - _speed;
	}
	else
	{
		this->_movement.y = _direction.y + _speed;
	}
}

void Ball::set_position()
{
	Vector2fSurcharged new_position(this->_position.x, this->_position.y);
	_position = new_position;
	this->setPosition(new_position);
}

void Ball::set_position(float x, float y)
{
	Vector2fSurcharged new_position(x, y);
	_position = new_position;
	this->setPosition(new_position);
}