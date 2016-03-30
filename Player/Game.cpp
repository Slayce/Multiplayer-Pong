#include "Game.h"
#include "NetworkManager.h"

using namespace std;

Game::Game(Port server_tcp_port, unsigned int const fps, unsigned int const window_width, unsigned int const window_height) :
_network_manager(server_tcp_port),
_fps(fps),
_window(sf::VideoMode(window_width, window_height), "Pong", sf::Style::Close | sf::Style::Titlebar),
_is_game_ended(false),
_thread_receive_data(&Game::receive_data, this),
_thread_send_inputs(&Game::send_inputs, this),
_thread_read_inputs(&Game::read_inputs, this)
{}

void Game::init()
{
	_network_manager.connect();

	_last_input.key.code = sf::Keyboard::Unknown;

	_main_clock.restart();
}

void Game::launch()
{
	_thread_receive_data.launch();
	_thread_send_inputs.launch();
	///_thread_read_inputs.launch();

	while (!_is_game_ended)
	{
		if (_main_clock.getElapsedTime().asSeconds() >= (1 / _fps))
		{
			display();
			_main_clock.restart();
			read_inputs();
			//send_inputs();
		}
	}
	gameEnded(_win_or_lose);

	_thread_receive_data.wait();
	_thread_send_inputs.wait();
	_thread_read_inputs.wait();
}

void Game::choose_racket_color()
{
	string color;
	cout << "Choisissez votre couleur :" << endl <<
		"1:Blanc" << endl <<
		"2:Jaune" << endl <<
		"3:Bleu" << endl <<
		"4:Cyan" << endl <<
		"5:Magenta" << endl <<
		"6:Rouge" << endl <<
		"7:Vert" << endl;
	cin >> color;
		
	int int_color = stoi(color);

	switch (int_color)
	{
	case 1:
		
		break;
	default:
		break;
	}
}

void Game::receive_data()
{
	while (!_is_game_ended)
	{
		struct NetworkManager::DataReceived data_received(_network_manager.receive_data());

		sf::Lock lock(_mutex);

		if (data_received.win_or_lose == 0) //si ce sont des données du jeu en cours
		{
			_ball = data_received.ball;
			_racket_one = data_received.racket_one;
			_racket_two = data_received.racket_two;
		}
		else //si le jeu est terminé
		{
			_is_game_ended = true;
			switch (data_received.win_or_lose)
			{
			case 1:
				_win_or_lose = Win;
				break;
			case 2:
				_win_or_lose = Lose;
				break;
			}
		}
		_sleep(1);
	}
}

void Game::display()
{
	sf::Lock lock(_mutex);

	_window.clear(sf::Color::Black);

	_racket_one.set_position();
	_racket_two.set_position();

	_window.draw(_ball);
	_window.draw(_racket_one);
	_window.draw(_racket_two);

	_window.display();
}

void Game::read_inputs()
{
	///while (!_is_game_ended)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //flèche gauche : déplacement du vaisseau à gauche si il n'est pas sur le bord gauche de l'écran
		{
			_last_input.key.code = sf::Keyboard::Left;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) //flèche droite : déplacement du vaisseau à doite si il n'est pas sur le bord droit de l'écran
		{
			_last_input.key.code = sf::Keyboard::Right;
		}

		sf::Event event;
		while (_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)  //clic sur la croix en haut à droite : fenêtre fermée
			{
				_window.close();
				_is_game_ended = true;
			}
		}
		_sleep(1);
	}
}

void Game::send_inputs()
{
	while (!_is_game_ended)
	{
		if (_last_input.key.code != sf::Keyboard::Unknown)
		{
			_network_manager.send_inputs(_last_input);
			_last_input.key.code = sf::Keyboard::Unknown;
		}
		_sleep(1);
	}
}

void Game::gameEnded(WinOrLose win_or_lose)
{
	sf::Lock lock(_mutex);

	sf::Text end_text_display;
	sf::Font font;

	if (!font.loadFromFile("../files/fonts/molot.ttf"))
		cout << "Impossible de charger la police de caractere" << endl;

	string text;

	_network_manager.send_feedback_game_ended();

	switch (win_or_lose)
	{
	case Win:
		text = "You win !";
		break;
	case Lose:
		text = "You lose...";
		break;
	}

	const unsigned int window_width(_window.getSize().x), window_height(_window.getSize().y);

	_window.clear(sf::Color::Black);

	end_text_display.setCharacterSize(100);
	end_text_display.setString(text);
	end_text_display.setFont(font);
	end_text_display.setColor(sf::Color::Red);
	end_text_display.setStyle(sf::Text::Bold);
	end_text_display.setPosition((window_width - end_text_display.getGlobalBounds().width) / 2, (window_height - end_text_display.getGlobalBounds().height) / 2);

	_window.draw(end_text_display);

	_window.display();

	_sleep(4000);
}