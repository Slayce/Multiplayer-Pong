#ifndef DEF_GAME
#define DEF_GAME

#include <sfml/Window/Event.hpp>
#include <SFML\System.hpp>
#include <array>
#include "Ball.h"
#include "Racket.h"
#include "NetworkManager.h"
#include "EventKeyPressed.h"
#include "Vector2fSurcharged.h"

class Game
{
public:
	typedef unsigned short const Port;

	Game(Port local_tcp_port, Port local_udp_port,
		Vector2fSurcharged rackets_dimensions, float default_rackets_speed,
		float ball_radius, float default_ball_speed, Vector2fSurcharged default_ball_direction,
		float const window_width, float const window_height);
	void init();
	void lauch();
	void choose_color();
	void update();
	void move_rackets();
	void move_ball();
	void check_collisions();
	void check_goal();
	void read_inputs();
	void send_data();
	bool isGameContinued() { return _is_game_continued; }

private:
	void setIsGameContinued(bool is_game_continued) { _is_game_continued = is_game_continued; }
	
	Ball _ball;
	Racket _racket_one, _racket_two;
	NetworkManager _network_manager;
	bool _is_game_ended,
		_is_game_continued;

	bool _is_threads_terminated;
	Vector2fSurcharged _window_dimensions;
	EventKeyPressed _last_inputs_one,
		_last_inputs_two;

	sf::Thread _thread_read_inputs,
		_thread_send_data;

	sf::Mutex _mutex;

	sf::Clock _main_clock, _move_clock, _collision_clock;
};

#endif