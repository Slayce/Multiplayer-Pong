#ifndef DEF_GAME
#define DEF_GAME

#include <sfml/graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <map>
#include <string>
#include "Ball.h"
#include "Racket.h"
#include "NetworkManager.h"
#include "Vector2fSurcharged.h"
#include "EventKeyPressed.h"

class Game
{
public:
	typedef unsigned short const Port;

	enum WinOrLose { Win, Lose };

	Game(Port server_tcp_port,
		unsigned int const fps, unsigned int const window_width, unsigned int const window_height);
	void init();
	void launch();
	void choose_racket_color();
	void receive_data();
	void display();
	void read_inputs();
	void send_inputs();
	void gameEnded(WinOrLose win_or_lose);
	bool IsGameRelaunched() { return _is_game_relaunched; }

private:
	void setIsGameRelaunched(bool is_game_relaunched) { _is_game_relaunched = is_game_relaunched; }

	NetworkManager _network_manager;
	sf::RenderWindow _window;
	Ball _ball;
	Racket _racket_one,
		   _racket_two;
	sf::Clock _main_clock;
	unsigned int const _fps;

	bool _is_game_ended,
		_is_game_relaunched;

	sf::Thread _thread_receive_data,
		_thread_send_inputs;

	sf::Mutex _mutex;

	EventKeyPressed _last_input;

	WinOrLose _win_or_lose;
};

#endif