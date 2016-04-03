#include "Game.h"
#include <iostream>

using namespace std;

Game::Game(Port local_tcp_port, Port local_udp_port, Vector2fSurcharged rackets_dimensions, float default_rackets_speed, float ball_radius, float default_ball_speed, Vector2fSurcharged default_ball_direction, float const window_width, float const window_height) :
_racket_one(rackets_dimensions, default_rackets_speed),
_racket_two(rackets_dimensions, default_rackets_speed),
_ball(ball_radius, default_ball_direction, default_ball_speed),
_is_game_continued(false),
_window_dimensions(window_width, window_height),
_network_manager(local_tcp_port, local_udp_port),
_thread_read_inputs(&Game::read_inputs, this),
_thread_send_data(&Game::send_data, this)
{}

void Game::init()
{
	if (!_is_game_continued)
	{
		_network_manager.connect(); //recherche de deux joueurs
		choose_color();
	}

	_is_game_ended = false;
	_is_threads_terminated = false;

	_ball.set_position((_window_dimensions.x / 2) - _ball.getRadius(), _window_dimensions.y / 2);

	_racket_one.set_position((_window_dimensions.x / 2) - (_racket_one.getSize().x / 2), (_window_dimensions.y - _racket_one.getSize().y) - 10);
	_racket_one.set_dimensions(_racket_one.getSize());

	_racket_two.set_position((_window_dimensions.x / 2) - (_racket_two.getSize().x / 2), 10);
	_racket_two.set_dimensions(_racket_two.getSize());

	_main_clock.restart();
	_collision_clock.restart();
	_move_clock.restart();
}

void Game::lauch()
{
	_thread_read_inputs.launch();
	_thread_send_data.launch();

	while (!_is_game_ended)
	{
		update();
		check_collisions();
		///send_data();
		///read_inputs();
		check_goal();
	}

	_thread_read_inputs.terminate();
	_thread_send_data.terminate();
}

void Game::choose_color()
{
	string s_color1, s_color2;

	cout << "Liste des couleurs: " << endl <<
		"1:Blanc" << endl <<
		"2:Jaune" << endl <<
		"3:Bleu" << endl <<
		"4:Cyan" << endl <<
		"5:Magenta" << endl <<
		"6:Rouge" << endl <<
		"7:Vert" << endl;

	cout << "Choisissez la couleur de P1:" << endl;
	cin >> s_color1;

	cout << "Choisissez la couleur de P2:" << endl;
	cin >> s_color2;

	int int_color1 = stoi(s_color1);
	int int_color2 = stoi(s_color2);

	sf::Color color1, color2;
	switch (int_color1)
	{
	case 1:
		color1 = sf::Color::White;
		break;
	case 2:
		color1 = sf::Color::Yellow;
		break;
	case 3:
		color1 = sf::Color::Blue;
		break;
	case 4:
		color1 = sf::Color::Cyan;
		break;
	case 5:
		color1 = sf::Color::Magenta;
		break;
	case 6:
		color1 = sf::Color::Red;
		break;
	case 7:
		color1 = sf::Color::Green;
		break;
	default:
		break;
	}
	switch (int_color2)
	{
	case 1:
		color2 = sf::Color::White;
		break;
	case 2:
		color2 = sf::Color::Yellow;
		break;
	case 3:
		color2 = sf::Color::Blue;
		break;
	case 4:
		color2 = sf::Color::Cyan;
		break;
	case 5:
		color2 = sf::Color::Magenta;
		break;
	case 6:
		color2 = sf::Color::Red;
		break;
	case 7:
		color2 = sf::Color::Green;
		break;
	default:
		break;
	}
	_racket_one.setFillColor(color1);
	_racket_two.setFillColor(color2);

	_racket_one.set_color(color1);
	_racket_two.set_color(color2);
}

void Game::update()
{
	sf::Lock lock(_mutex);
	if (_move_clock.getElapsedTime().asMilliseconds() >= 11)
	{
		_ball.set_speed(1 + _main_clock.getElapsedTime().asSeconds() * 0.02);
		move_rackets();
		move_ball();
		_move_clock.restart();
	}
}

void Game::move_rackets()
{
	
	if (_last_inputs_one.key.code != sf::Keyboard::Unknown)
	{
		switch (_last_inputs_one.key.code)  //on déplace la raquette 1
		{
		case (sf::Keyboard::Left) :
			_racket_one.move_left();
			break;
		case (sf::Keyboard::Right) :
			_racket_one.move_right(_window_dimensions.x);
			break;
		default:
			break;
		}
		_last_inputs_one.key.code = sf::Keyboard::Unknown;
	}
	
	if (_last_inputs_two.key.code != sf::Keyboard::Unknown)
	{
		switch (_last_inputs_two.key.code)  //on déplace la raquette 2
		{
		case (sf::Keyboard::Left) :
			_racket_two.move_left();
			break;
		case (sf::Keyboard::Right) :
			_racket_two.move_right(_window_dimensions.x);
			break;
		default:
			break;
		}
		_last_inputs_two.key.code = sf::Keyboard::Unknown;
	}
}

void Game::move_ball()
{
	_ball.move_ball();
}

void Game::check_collisions()
{
	if (_collision_clock.getElapsedTime().asMilliseconds() >= 10) //on check les collisions toutes les secondes pour éviter les zig-zags
	{
		if (_ball.getGlobalBounds().intersects(_racket_one.getGlobalBounds()))
			_ball.bounce(Ball::Side::BOT);

		if (_ball.getGlobalBounds().intersects(_racket_two.getGlobalBounds()))
			_ball.bounce(Ball::Side::TOP);

		if (_ball.getPosition().x <= 0)
			_ball.bounce(Ball::Side::LEFT);

		if (_ball.getPosition().x + (_ball.getRadius() * 2) >= _window_dimensions.x)
			_ball.bounce(Ball::Side::RIGHT);

		_collision_clock.restart();
	}
}

void Game::check_goal()
{
	if (_ball.getPosition().y + (_ball.getRadius() * 2) >= _window_dimensions.y) //si la balle est dans le camp de P1
	{
		_is_threads_terminated = true;
		_is_game_continued = _network_manager.send_win(NetworkManager::Player2);
		_is_game_ended = true;
	}
		
	if (_ball.getPosition().y <= 0) //si la balle est dans le camp de P2
	{
		_is_threads_terminated = true;
		_is_game_continued = _network_manager.send_win(NetworkManager::Player1);
		_is_game_ended = true;
	}
}

void Game::read_inputs() //récupère les données envoyées par les players
{
	while (!_is_threads_terminated)
	{
		NetworkManager::InputReceived received_input = _network_manager.receive_input();

		if (received_input.event_key_pressed.key.code != sf::Keyboard::Unknown)
		{
			switch (received_input.player)
			{
			case NetworkManager::Player1:
				_last_inputs_one = received_input.event_key_pressed;
				break;
			case NetworkManager::Player2:
				_last_inputs_two = received_input.event_key_pressed;
				break;
			}
		}
		_sleep(1);
	}
}

void Game::send_data()
{
	while (!_is_threads_terminated)
	{
		_network_manager.send_data(_ball, _racket_one, _racket_two);
		
		_sleep(1);
	}
}