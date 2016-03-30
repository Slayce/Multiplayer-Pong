#include <sfml/graphics.hpp>
#include "Game.h"
#include "Vector2fSurcharged.h"

typedef unsigned short const Port;

//Rackets properties
Vector2fSurcharged rackets_dimensions(75, 20);
float default_rackets_speed = 3;


//Ball properties
float ball_radius = 10;
float default_ball_speed = 1;
Vector2fSurcharged default_ball_direction(1, 1);


//Window properties
float const window_width = 600;
float const window_height = 900;


//Network properties (BE AWARE THAT YOU MAY BREAK THE PROGRAM IF YOU CHANGE THESE ENTRIES)
Port local_tcp_port = 53000; //port où les players se connectent
Port local_udp_port = 53000; //port à surveiller pour les données UDP des 2 players


int main()
{
	Game game(local_tcp_port, local_udp_port, rackets_dimensions, default_rackets_speed, ball_radius, default_ball_speed, default_ball_direction, window_width, window_height);
	game.init();
	game.lauch();

	return 0;
}