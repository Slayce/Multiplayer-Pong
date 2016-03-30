#ifndef DEF_NETWORKMANAGER
#define DEF_NETWORKMANAGER

#include <sfml/network.hpp>
#include <iostream>
#include <map>
#include <string>
#include "Vector2fSurcharged.h"
#include "EventKeyPressed.h"
#include "Ball.h"
#include "Racket.h"

class NetworkManager
{
public:
	struct DataReceived
	{
		Ball ball;
		Racket racket_one, racket_two;
		sf::Uint8 win_or_lose;
	};

	typedef unsigned short Port;

	NetworkManager(Port server_tcp_port);
	void connect();
	void receive_udp_port();
	DataReceived receive_data();
	void send_inputs(EventKeyPressed input);
	void send_feedback_game_ended();

private:
	sf::TcpSocket _tcp_server;
	Port _server_tcp_port,
		_server_udp_port;
	sf::IpAddress _ip_server;
};

#endif