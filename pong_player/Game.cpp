#include "Game.h"
#include "NetworkManager.h"

using namespace std;

Game::Game(Port server_tcp_port, unsigned int const fps, unsigned int const window_width, unsigned int const window_height) :
_network_manager(server_tcp_port),
_fps(fps),
_is_game_relaunched(false),
_window(sf::VideoMode(window_width, window_height), "Pong", sf::Style::Close | sf::Style::Titlebar),
_thread_receive_data(&Game::receive_data, this),
_thread_send_inputs(&Game::send_inputs, this)
{}

void Game::init()
{
	if (!_is_game_relaunched)
	{
		_network_manager.connect();
	}

	_is_game_ended = false;

	_last_input.key.code = sf::Keyboard::Unknown;

	_main_clock.restart();
}

void Game::launch()
{
	_thread_receive_data.launch();
	_thread_send_inputs.launch();

	while (!_is_game_ended)
	{
		if (_main_clock.getElapsedTime().asSeconds() >= (1 / _fps))
		{
			display();
			read_inputs();
			_main_clock.restart();
		}
	}

	_thread_receive_data.wait();
	_thread_send_inputs.wait();

	gameEnded(_win_or_lose);
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

	sf::Text end_text_display, play_again_display, yes_display, no_display;
	sf::Font font;

	string text;

	if (!font.loadFromFile("../files/fonts/molot.ttf"))
		cout << "Impossible de charger la police de caractere" << endl;

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
	end_text_display.setPosition((window_width - end_text_display.getGlobalBounds().width) / 2, (window_height - end_text_display.getGlobalBounds().height) / 3);

	play_again_display.setCharacterSize(60);
	play_again_display.setString("Play again ?");
	play_again_display.setFont(font);
	play_again_display.setColor(sf::Color::White);
	play_again_display.setPosition((window_width - play_again_display.getGlobalBounds().width) / 2, (window_height - play_again_display.getGlobalBounds().height) / 2);

	yes_display.setCharacterSize(60);
	yes_display.setString("Yes");
	yes_display.setFont(font);
	yes_display.setColor(sf::Color::Green);
	yes_display.setPosition((window_width - yes_display.getGlobalBounds().width) / 4, (window_height - yes_display.getGlobalBounds().height) / 3 * 2);

	no_display.setCharacterSize(60);
	no_display.setString("No");
	no_display.setFont(font);
	no_display.setColor(sf::Color::Blue);
	no_display.setPosition((window_width - no_display.getGlobalBounds().width) / 4 * 3, (window_height - no_display.getGlobalBounds().height) / 3 * 2);

	_window.draw(end_text_display);
	_window.draw(play_again_display);
	_window.draw(yes_display);
	_window.draw(no_display);

	_window.display();

	bool is_game_relaunched = false;

	bool is_clicked = false;

	while (!is_clicked)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2i mouse_pos = sf::Mouse::getPosition(_window);

			is_clicked = true;

			bool is_yes_clicked = mouse_pos.x >= yes_display.getGlobalBounds().left
				&& mouse_pos.x <= yes_display.getGlobalBounds().left + yes_display.getGlobalBounds().width
				&& mouse_pos.y >= yes_display.getGlobalBounds().top
				&& mouse_pos.y <= yes_display.getGlobalBounds().top + yes_display.getGlobalBounds().height;

			bool is_no_clicked = mouse_pos.x >= no_display.getGlobalBounds().left
				&& mouse_pos.x <= no_display.getGlobalBounds().left + no_display.getGlobalBounds().width
				&& mouse_pos.y >= no_display.getGlobalBounds().top
				&& mouse_pos.y <= no_display.getGlobalBounds().top + no_display.getGlobalBounds().height;

			if (is_yes_clicked)
			{
				is_game_relaunched = true;
			}
			else if (is_no_clicked)
			{
				is_game_relaunched = false;
			}
			else
				is_clicked = false;
		}
	}

	setIsGameRelaunched(is_game_relaunched);

	_network_manager.send_is_game_continued(is_game_relaunched);

	is_game_relaunched = _network_manager.receive_is_game_continued();
}