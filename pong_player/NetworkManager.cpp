#include "NetworkManager.h"

using namespace std;

NetworkManager::NetworkManager(Port server_tcp_port) :
_server_tcp_port(server_tcp_port)
{}

void NetworkManager::connect()
{
	string ip;
	cout << "Adresse IP du serveur : (ex : 92.136.202.213)" << endl;
	cin >> ip;

	_ip_server = ip;

	if (_tcp_server.connect(_ip_server, _server_tcp_port) == sf::Socket::Done)
		cout << "Connecte !" << endl;

	receive_udp_port();
}

void NetworkManager::receive_udp_port()
{
	_tcp_server.setBlocking(true);

	sf::Packet pkt_udp_port;

	_tcp_server.receive(pkt_udp_port);

	pkt_udp_port >> _server_udp_port;
}

NetworkManager::DataReceived NetworkManager::receive_data()
{
	_tcp_server.setBlocking(true);

	sf::Packet data;
	struct DataReceived data_received;
	sf::Uint8 which_data;

	_tcp_server.receive(data);

	data >> which_data >> data_received.ball >> data_received.racket_one >> 
		data_received.racket_two >> data_received.win_or_lose;

	data_received.ball.setPosition(data_received.ball.get_position());
	data_received.ball.setRadius(data_received.ball.get_radius());

	data_received.racket_one.setSize(data_received.racket_one.get_dimensions());
	data_received.racket_two.setSize(data_received.racket_two.get_dimensions());

	data_received.racket_one.setFillColor(data_received.racket_one.get_color());
	data_received.racket_two.setFillColor(data_received.racket_two.get_color());

	return data_received;
}

void NetworkManager::send_inputs(EventKeyPressed input)
{
	sf::UdpSocket _udp_server;

	sf::Packet packet;

	packet << input;

	_udp_server.send(packet, _ip_server, _server_udp_port);
}

void NetworkManager::send_is_game_continued(bool is_game_continued)
{
	sf::Packet data;

	data << is_game_continued;

	if (_tcp_server.send(data) != sf::Socket::Done)
		cout << "Feedback was not sent to server" << endl;
}

bool NetworkManager::receive_is_game_continued()
{
	_tcp_server.setBlocking(true);

	sf::Packet data;
	sf::Uint8 which_data(0);
	
	bool is_game_continued = false;

	while (which_data != 2)
	{
		_tcp_server.receive(data);
		data >> which_data >> is_game_continued;
	}

	return is_game_continued;
}