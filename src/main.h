#ifndef MAIN_H
#define MAIN_H

#define GAME_NAME 				"Game Name"
#define WINDOW_WIDTH  			800
#define WINDOW_HEIGHT 			600

#define rrender_max(a, b) 			(((a) > (b)) ? a : b)
#define rrender_min(a, b) 			(((a) < (b)) ? a : b)
#define rrender_abs(a) 				(((a) < 0) ? (-a) : (a))
#define rrender_clamp(x, min, max)  (rrender_min(rrender_max((x), (min)), (max)))

// These are just here for convenience (the color format is 32bit - ABGR, in my machine at least)
#define RRENDER_COLOR_LIGHT_LIGHT_RED		0x009191ff
#define RRENDER_COLOR_LIGHT_RED  			0x004c4cff
#define RRENDER_COLOR_RED  					0x001414ff
#define RRENDER_COLOR_DARK_RED  			0x000000aa
#define RRENDER_COLOR_DARK_DARK_RED  		0x00000070
#define RRENDER_COLOR_LIGHT_LIGHT_GREEN  	0x007fff7f
#define RRENDER_COLOR_LIGHT_GREEN  			0x0051ff51
#define RRENDER_COLOR_GREEN  				0x0016e216
#define RRENDER_COLOR_DARK_GREEN  			0x00008700
#define RRENDER_COLOR_DARK_DARK_GREEN  		0x00005400
#define RRENDER_COLOR_LIGHT_LIGHT_BLUE  	0x00ff7777
#define RRENDER_COLOR_LIGHT_BLUE  			0x00ff4f4f
#define RRENDER_COLOR_BLUE  				0x00ea1c1c
#define RRENDER_COLOR_DARK_BLUE  			0x00890000
#define RRENDER_COLOR_DARK_DARK_BLUE  		0x005b0000
#define RRENDER_COLOR_LIGHT_LIGHT_YELLOW  	0x0085fcfc
#define RRENDER_COLOR_LIGHT_YELLOW  		0x005bffff
#define RRENDER_COLOR_YELLOW 				0x0014e8e8
#define RRENDER_COLOR_DARK_YELLOW  			0x00009e9e
#define RRENDER_COLOR_DARK_DARK_YELLOW  	0x00006d6d
#define RRENDER_COLOR_LIGHT_LIGHT_CYAN  	0x00fcfc96
#define RRENDER_COLOR_LIGHT_CYAN  			0x00fcfc50
#define RRENDER_COLOR_CYAN  				0x00e5e519
#define RRENDER_COLOR_DARK_CYAN  			0x009e9e00
#define RRENDER_COLOR_DARK_DARK_CYAN  		0x00666600
#define RRENDER_COLOR_LIGHT_LIGHT_MAGENTA  	0x00fc8dfc
#define RRENDER_COLOR_LIGHT_MAGENTA  		0x00ff4fff
#define RRENDER_COLOR_MAGENTA  				0x00e812e8
#define RRENDER_COLOR_DARK_MAGENTA  		0x00aa00aa
#define RRENDER_COLOR_DARK_DARK_MAGENTA  	0x006b006b
#define RRENDER_COLOR_LIGHT_LIGHT_GRAY  	0x00cecece
#define RRENDER_COLOR_LIGHT_GRAY  			0x00a6a6a8
#define RRENDER_COLOR_GRAY  				0x00757575
#define RRENDER_COLOR_DARK_GRAY  			0x00555556
#define RRENDER_COLOR_DARK_DARK_GRAY  		0x00303030
#define RRENDER_COLOR_WHITE  				0x00ffffff
#define RRENDER_COLOR_BLACK  				0x00000000

bool init();
void create_window();
void handle_input();
void render_begin();
void render_end();
void render_graphics();
void draw_background(unsigned int* framebuffer, unsigned int color);
void draw_AABB(unsigned int* framebuffer, int minX, int minY, int maxX, int maxY, unsigned int color);
void draw_line(unsigned int* framebuffer, int aX, int aY, int bX, int bY, unsigned int color);
void draw_pixel(unsigned int* framebuffer, int x, int y, unsigned int color);
void quit();

#endif
