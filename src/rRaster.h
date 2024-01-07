#ifndef RRASTER_H
#define RRASTER_H

#include <iostream>
#include "SDL.h"

class rRaster{
public:
	rRaster(SDL_Window* window, int context_width, int context_height);
	~rRaster();
	void raster_background(unsigned int color);
	void raster_AABB(int minX, int minY, int maxX, int maxY, unsigned int color);
	void raster_AABB_filled(int minX, int minY, int maxX, int maxY, unsigned int color);
	void raster_line(int aX, int aY, int bX, int bY, unsigned int color);
	void raster_circle(int cX, int cY, float radius, unsigned int color);
	void raster_triangle(int x0, int y0, int x1, int y1, int x2, int y2, unsigned int color);
	void raster_pixel(int x, int y, unsigned int color);
	void raster_begin();
	void raster_end();

	int raster_width;
	int raster_height;

private:
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	unsigned int* framebuffer;
};

#define RRASTER_MAX(a, b) 			(((a) > (b)) ? a : b)
#define RRASTER_MIN(a, b) 			(((a) < (b)) ? a : b)
#define RRASTER_ABS(a) 				(((a) < 0) ? (-a) : (a))
#define RRASTER_CLAMP(x, min, max)  (RRASTER_MIN(RRASTER_MAX((x), (min)), (max)))

// These are just here for convenience (the color format is 32bit - ABGR, in my machine at least)
#define RRASTER_COLOR_LIGHT_LIGHT_RED		0x009191ff
#define RRASTER_COLOR_LIGHT_RED  			0x004c4cff
#define RRASTER_COLOR_RED  					0x001414ff
#define RRASTER_COLOR_DARK_RED  			0x000000aa
#define RRASTER_COLOR_DARK_DARK_RED  		0x00000070
#define RRASTER_COLOR_LIGHT_LIGHT_GREEN  	0x007fff7f
#define RRASTER_COLOR_LIGHT_GREEN  			0x0051ff51
#define RRASTER_COLOR_GREEN  				0x0016e216
#define RRASTER_COLOR_DARK_GREEN  			0x00008700
#define RRASTER_COLOR_DARK_DARK_GREEN  		0x00005400
#define RRASTER_COLOR_LIGHT_LIGHT_BLUE  	0x00ff7777
#define RRASTER_COLOR_LIGHT_BLUE  			0x00ff4f4f
#define RRASTER_COLOR_BLUE  				0x00ea1c1c
#define RRASTER_COLOR_DARK_BLUE  			0x00890000
#define RRASTER_COLOR_DARK_DARK_BLUE  		0x005b0000
#define RRASTER_COLOR_LIGHT_LIGHT_YELLOW  	0x0085fcfc
#define RRASTER_COLOR_LIGHT_YELLOW  		0x005bffff
#define RRASTER_COLOR_YELLOW 				0x0014e8e8
#define RRASTER_COLOR_DARK_YELLOW  			0x00009e9e
#define RRASTER_COLOR_DARK_DARK_YELLOW  	0x00006d6d
#define RRASTER_COLOR_LIGHT_LIGHT_CYAN  	0x00fcfc96
#define RRASTER_COLOR_LIGHT_CYAN  			0x00fcfc50
#define RRASTER_COLOR_CYAN  				0x00e5e519
#define RRASTER_COLOR_DARK_CYAN  			0x009e9e00
#define RRASTER_COLOR_DARK_DARK_CYAN  		0x00666600
#define RRASTER_COLOR_LIGHT_LIGHT_MAGENTA  	0x00fc8dfc
#define RRASTER_COLOR_LIGHT_MAGENTA  		0x00ff4fff
#define RRASTER_COLOR_MAGENTA  				0x00e812e8
#define RRASTER_COLOR_DARK_MAGENTA  		0x00aa00aa
#define RRASTER_COLOR_DARK_DARK_MAGENTA  	0x006b006b
#define RRASTER_COLOR_LIGHT_LIGHT_GRAY  	0x00cecece
#define RRASTER_COLOR_LIGHT_GRAY  			0x00a6a6a8
#define RRASTER_COLOR_GRAY  				0x00757575
#define RRASTER_COLOR_DARK_GRAY  			0x00555556
#define RRASTER_COLOR_DARK_DARK_GRAY  		0x00303030
#define RRASTER_COLOR_WHITE  				0x00ffffff
#define RRASTER_COLOR_BLACK  				0x00000000

#endif // RRASTER_H