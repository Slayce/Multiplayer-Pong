#include "Racket.h"

Racket::Racket() : sf::RectangleShape(),
_speed(0)
{}

Racket::Racket(Vector2fSurcharged dimensions, float speed) : sf::RectangleShape(dimensions),
_speed(speed),
_dimensions(dimensions)
{}

void Racket::move_right(float window_width)
{
	if (this->_position.x + this->getGlobalBounds().width < window_width)
	{
		float x = this->_position.x + _speed;
		float y = this->_position.y;
		this->set_position(x, y);
	}
}

void Racket::move_left()
{
	if (this->_position.x > 0)
	{
		float x = this->_position.x - _speed;
		float y = this->_position.y;
		this->set_position(x, y);
	}
}

void Racket::set_position()
{
	Vector2fSurcharged new_position(this->_position.x, this->_position.y);
	_position = new_position;
	this->setPosition(new_position);
}

void Racket::set_position(float x, float y)
{
	Vector2fSurcharged new_position(x, y);
	_position = new_position;
	this->setPosition(new_position);
}