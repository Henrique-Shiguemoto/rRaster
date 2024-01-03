#ifndef MAIN_H
#define MAIN_H

#define GAME_NAME 				"Game Name"
#define WINDOW_WIDTH  			800
#define WINDOW_HEIGHT 			600

bool init();
void create_window();
void handle_input();
void render_begin();
void render_end();
void render_graphics();
void draw_background(unsigned int* framebuffer, unsigned int color);
void draw_AABB(unsigned int* framebuffer, int minX, int minY, int maxX, int maxY, unsigned int color);
void draw_pixel(unsigned int* framebuffer, int x, int y, unsigned int color);
void quit();

#endif
