#ifndef DEF_NETWORKMANAGER
#define DEF_NETWORKMANAGER

#include <sfml/network.hpp>
#include <sfml/window.hpp>
#include <iostream>
#include <array>
#include "EventKeyPressed.h"
#include "Vector2fSurcharged.h"
#include "Ball.h"
#include "Racket.h"

class NetworkManager
{
public:
	typedef unsigned short const Port;

	enum Players { Player1, Player2 };

	struct InputReceived
	{
		Players player;
		EventKeyPressed event_key_pressed;
	};

	NetworkManager(Port local_tcp_port, Port local_udp_port);
	void connect();
	void send_udp_port();
	InputReceived receive_input();
	void send_data(Ball const &ball, Racket const &racket_one, Racket const &racket_two);

	void send_win(Players winner);

private:
	sf::TcpSocket _client_one_tcp, _client_two_tcp;
	sf::IpAddress _ip_client_one, _ip_client_two;
	Port _local_tcp_port,
		_local_udp_port;
};

#endif