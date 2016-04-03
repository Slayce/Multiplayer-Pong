#include "NetworkManager.h"

using namespace std;

NetworkManager::NetworkManager(Port local_tcp_port, Port local_udp_port) :
_local_tcp_port(local_tcp_port),
_local_udp_port(local_udp_port)
{}

void NetworkManager::connect()
{
	sf::TcpListener listener;
	listener.setBlocking(true);

	if (listener.listen(_local_tcp_port) == sf::Socket::Done)
		cout << "Recherche de joueurs..." << endl;

	if (listener.accept(_client_one_tcp) == sf::Socket::Done)
		cout << "Premier joueur : " << _client_one_tcp.getRemoteAddress() << endl;

	if (listener.accept(_client_two_tcp) == sf::Socket::Done)
		cout << "Deuxieme joueur : " << _client_two_tcp.getRemoteAddress() << endl;

	_ip_client_one = _client_one_tcp.getRemoteAddress();
	_ip_client_two = _client_two_tcp.getRemoteAddress();

	send_udp_port();
}

void NetworkManager::send_udp_port()
{
	sf::Packet packet_udp_port;

	_client_one_tcp.setBlocking(true);
	_client_two_tcp.setBlocking(true);

	packet_udp_port << _local_udp_port;

	if (_client_one_tcp.send(packet_udp_port) != sf::Socket::Done)
		cout << "The ports was not sent to P1" << endl;

	if (_client_two_tcp.send(packet_udp_port) != sf::Socket::Done)
		cout << "The ports was not sent to P2" << endl;
}

NetworkManager::InputReceived NetworkManager::receive_input()
{
	NetworkManager::InputReceived input_received;

	sf::UdpSocket client_udp;

	client_udp.setBlocking(true);

	sf::Packet packet;
	EventKeyPressed input;
	unsigned short port;
	sf::IpAddress ip_client;
	int input_key_pressed;
	NetworkManager::Players player;

	client_udp.bind(_local_udp_port);

	client_udp.receive(packet, ip_client, port);

	packet >> input_key_pressed;

	switch (input_key_pressed)
	{
	case 71:
		input.key.code = sf::Keyboard::Left;
		break;
	case 72:
		input.key.code = sf::Keyboard::Right;
		break;
	default:
		input.key.code = sf::Keyboard::Unknown;
		break;
	}

	if (ip_client == _ip_client_one)
	{
		player = NetworkManager::Player1;
	}
	else if (ip_client == _ip_client_two)
	{
		player = NetworkManager::Player2;
	}

	input_received.event_key_pressed = input;
	input_received.player = player;

	return input_received;
}

void NetworkManager::send_data(Ball const &ball, Racket const &racket_one, Racket const &racket_two)
{
	sf::Packet data;
	sf::Uint8 which_data(0), win_or_lose(0); //ce n'est pas la fin du jeu

	data << which_data << ball << racket_one << racket_two << win_or_lose;

	if (_client_one_tcp.send(data) != sf::Socket::Done)
		cout << "Data were not sent to P1" << endl;

	if (_client_two_tcp.send(data) != sf::Socket::Done)
		cout << "Data were not sent to P2" << endl;
}

bool NetworkManager::send_win(NetworkManager::Players winner)
{
	_client_one_tcp.setBlocking(false);
	_client_two_tcp.setBlocking(false);

	sf::Packet winner_packet, loser_packet;
	sf::Uint8 which_data(1), win(1), lose(2);

	Ball ball;
	Racket racket1, racket2;

	winner_packet << which_data << ball << racket1 << racket2 << win;
	loser_packet << which_data << ball << racket1 << racket2 << lose;

	sf::Clock timeout;

	timeout.restart();

	sf::Packet feedback1, feedback2;

	switch (winner)
	{
	case Player1: //si P1 a gagné
		while (timeout.getElapsedTime().asSeconds() < 15 && (_client_one_tcp.receive(feedback1) != sf::Socket::Done || _client_two_tcp.receive(feedback2) != sf::Socket::Done)) //tant que les 2 joueurs n'ont pas reçu leur msg, on attend 15 sec
		{
			if (_client_one_tcp.send(winner_packet) != sf::Socket::Done)
				cout << "winner_packet was not sent to P1" << endl;

			if (_client_two_tcp.send(loser_packet) != sf::Socket::Done)
				cout << "loser_packet was not sent to P2" << endl;

			_sleep(10);
		}
		break;

	case Player2: //si P2 a gagné
		while (timeout.getElapsedTime().asSeconds() < 15 && (_client_two_tcp.receive(feedback1) != sf::Socket::Done || _client_one_tcp.receive(feedback2) != sf::Socket::Done)) //tant que les 2 joueurs n'ont pas reçu leur msg, on attend 15 sec
		{
			if (_client_one_tcp.send(loser_packet) != sf::Socket::Done)
				cout << "loser_packet was not sent to P1" << endl;

			if (_client_two_tcp.send(winner_packet) != sf::Socket::Done)
				cout << "winner_packet was not sent to P2" << endl;

			_sleep(10);
		}
		break;
	}

	bool p1_is_game_continued,
		p2_is_game_continued;

	feedback1 >> p1_is_game_continued;
	feedback2 >> p2_is_game_continued;
	
	bool is_game_continued = send_is_game_continued(p1_is_game_continued, p2_is_game_continued);

	return is_game_continued;
}

bool NetworkManager::send_is_game_continued(bool p1_choice, bool p2_choice)
{
	_client_one_tcp.setBlocking(true);
	_client_two_tcp.setBlocking(true);

	bool is_game_continued = false;

	if (p1_choice && p2_choice)
	{
		is_game_continued = true;
	}

	sf::Packet data;
	sf::Uint8 which_data(2);

	data << which_data << is_game_continued;

	if (_client_one_tcp.send(data) != sf::Socket::Done)
		cout << "is_game_continued were not sent to P1" << endl;

	if (_client_two_tcp.send(data) != sf::Socket::Done)
		cout << "Data were not sent to P2" << endl;

	return is_game_continued;
}