#include "Game.h"

typedef unsigned short const Port;

//Network properties (BE AWARE THAT YOU MAY BREAK THE PROGRAM IF YOU CHANGE THESE DATA)
Port server_tcp_port = 53000; //port du serveur où se connecter en TCP


//Window properties
unsigned int const fps = 60;
unsigned int const window_width = 600;
unsigned int const window_height = 900;


int main()
{
	bool is_game_continued = true;

	Game game(server_tcp_port, fps, window_width, window_height);
	
	while (is_game_continued)
	{
		game.init();
		game.launch();
		is_game_continued = game.IsGameRelaunched();
	}

	return 0;
}