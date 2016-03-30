#include "Game.h"

typedef unsigned short const Port;

//Network properties (BE AWARE THAT YOU MAY BREAK THE PROGRAM IF YOU CHANGE THESE ENTRIES)
Port server_tcp_port = 53000; //port du serveur où se connecter en TCP


//Window properties
unsigned int const fps = 60;
unsigned int const window_width = 600;
unsigned int const window_height = 900;


int main()
{
	Game game(server_tcp_port, fps, window_width, window_height);
	game.init();
	game.launch();

	return 0;
}