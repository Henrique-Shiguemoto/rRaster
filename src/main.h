#ifndef MAIN_H
#define MAIN_H

#define GAME_NAME 				"Game Name"
#define WINDOW_DEFAULT_WIDTH  	800
#define WINDOW_DEFAULT_HEIGHT 	600

bool init();
void create_window();
void handle_input();
void simulate_world();
void render_graphics();
void quit();

#endif
