#include <iostream>

#define SDL_MAIN_HANDLED
#include "SDL.h"

#include "main.h"

#include <math.h>

bool g_GameIsRunning = false;
SDL_Window* g_Window = nullptr;
SDL_Renderer* g_Renderer = nullptr;
SDL_Texture* g_Texture = nullptr;

unsigned int g_Framebuffer[WINDOW_WIDTH * WINDOW_HEIGHT] = {0};

int main(){
	if(!init()) return 1;
	create_window();
	
	while(g_GameIsRunning){
		handle_input();
		render_graphics();
	}
	quit();
	return 0;
}

bool init(){
	if(SDL_Init(SDL_INIT_VIDEO)){
		std::cout << SDL_GetError() << std::endl;
		return false;
	}
	
	g_GameIsRunning = true;
	
	return true;
}

void create_window(){
	if((g_Window = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN))) 
		std::cout << SDL_GetError() << std::endl;
	if((g_Renderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED)))
		std::cout << SDL_GetError() << std::endl;
	g_Texture = SDL_CreateTexture(g_Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
	if(!g_Texture){
		std::cout << SDL_GetError() << std::endl;
	}
}

void handle_input(){
	SDL_Event e;
	while(SDL_PollEvent(&e)){
		const unsigned char* keyboardState = SDL_GetKeyboardState(NULL);
		
		//keyboard stuff
		g_GameIsRunning = !keyboardState[SDL_SCANCODE_ESCAPE];
		
		//mouse stuff
		if(e.type == SDL_QUIT){
			g_GameIsRunning = false;
		}
	}
}

void render_begin(){
	SDL_SetRenderDrawColor(g_Renderer, 0, 0, 0, 255);
}

void render_end(){
	SDL_UpdateTexture(g_Texture, NULL, g_Framebuffer, (int)(sizeof(unsigned int) * WINDOW_WIDTH));
	SDL_RenderCopy(g_Renderer, g_Texture, NULL, NULL);
	SDL_RenderPresent(g_Renderer);
	SDL_RenderClear(g_Renderer);
}

void render_graphics(){
	render_begin();

	draw_background(g_Framebuffer, RRASTER_COLOR_BLUE);

	render_end();
}

void draw_background(unsigned int* framebuffer, unsigned int color){
	for(int i = 0; i < WINDOW_HEIGHT; i++)
		for (int j = 0; j < WINDOW_WIDTH; ++j)
			draw_pixel(g_Framebuffer, j, i, color);
}

void draw_AABB(unsigned int* framebuffer, int minX, int minY, int maxX, int maxY, unsigned int color){
	minX = RRASTER_CLAMP(minX, 0, WINDOW_WIDTH - 1);
	maxX = RRASTER_CLAMP(maxX, 0, WINDOW_WIDTH - 1);
	minY = RRASTER_CLAMP(minY, 0, WINDOW_HEIGHT - 1);
	maxY = RRASTER_CLAMP(maxY, 0, WINDOW_HEIGHT - 1);

	for (int i = minY; i < maxY; ++i){
		for (int j = minX; j < maxX; ++j){
			draw_pixel(g_Framebuffer, j, i, color);
		}
	}
}

void draw_line(unsigned int* framebuffer, int x0, int y0, int x1, int y1, unsigned int color){
	x0 = RRASTER_CLAMP(x0, 0, WINDOW_WIDTH - 1);
	x1 = RRASTER_CLAMP(x1, 0, WINDOW_WIDTH - 1);
	y0 = RRASTER_CLAMP(y0, 0, WINDOW_HEIGHT - 1);
	y1 = RRASTER_CLAMP(y1, 0, WINDOW_HEIGHT - 1);

	int dx = x1 - x0;
	int dy = y1 - y0;
	int cx = (dx > 0) ? 1 : -1;
	int cy = (dy > 0) ? 1 : -1;

	// vertical lines
	if(dx == 0){
		if(dy == 0) {
			draw_pixel(framebuffer, x0, y0, color);
			return;
		}
		
		// we're deciding if we go up or down
		// 		- if up,   then y1 is smaller than y0 (dy < 0), so we have to decrement y (cy = -1)
		// 		- if down, then y1 is bigger  than y0 (dy > 0), so we have to increment y (cy = +1)
		// also, we're multiplying the condition inequation by cy because we need to flip it when cy = -1
		for (int y = y0; cy * y < cy * y1; y += cy) draw_pixel(framebuffer, x0, y, color);
	}else if (RRASTER_ABS(dy / dx) < 1){
		// bresenham
		float error = 0.5f;
		float m = (float)dy / (float)dx; // rise over run

		int start_x = x0;
		int start_y = y0;
		int dest_x = x1;

		while(cx * start_x < cx * dest_x){
			draw_pixel(framebuffer, start_x, start_y, color);
			start_x += cx;
			error += RRASTER_ABS(m);
			if(error >= 0.5f){
				start_y += cy;
				error -= 1.0f;
			}
		}
	}else{
		// bresenham
		float error = 0.5f;
		float m = (float)dx / (float)dy; // run over rise

		int start_x = x0;
		int start_y = y0;
		int dest_x = x1;

		while(cx * start_x < cx * dest_x){
			draw_pixel(framebuffer, start_x, start_y, color);
			start_y += cy;
			error += RRASTER_ABS(m);
			if(error >= 0.5f){
				start_x += cx;
				error -= 1.0f;
			}
		}
	}
}

void draw_circle(unsigned int* framebuffer, int cX, int cY, float radius, unsigned int color){
	cX = RRASTER_CLAMP(cX, 0, WINDOW_WIDTH - 1);
	cY = RRASTER_CLAMP(cY, 0, WINDOW_HEIGHT - 1);

	// bresenham
	int start_x = 0;
	int start_y = -(int)radius;
	float radius2 = radius * radius;

	while(start_x <= -start_y){
		draw_pixel(framebuffer,  start_x + cX,  start_y + cY, color);
		draw_pixel(framebuffer,  start_x + cX, -start_y + cY, color);
		draw_pixel(framebuffer, -start_x + cX,  start_y + cY, color);
		draw_pixel(framebuffer, -start_x + cX, -start_y + cY, color);
		draw_pixel(framebuffer,  start_y + cY,  start_x + cX, color);
		draw_pixel(framebuffer, -start_y + cY,  start_x + cX, color);
		draw_pixel(framebuffer,  start_y + cY, -start_x + cX, color);
		draw_pixel(framebuffer, -start_y + cY, -start_x + cX, color);

		start_x++;

		float l2 = (float)(start_x * start_x) + (float)(start_y * start_y) - radius2;
		float ld2 = (float)(start_x * start_x) + (float)((start_y + 1) * (start_y + 1)) - radius2;
		float error_going_left = RRASTER_ABS(l2);
		float error_going_left_and_down = RRASTER_ABS(ld2);
		if(error_going_left_and_down < error_going_left){
			start_y++;
		}
	}
}

void draw_triangle(unsigned int* framebuffer, int x0, int y0, int x1, int y1, int x2, int y2, unsigned int color){
	draw_line(framebuffer, x0, y0, x1, y1, color);
	draw_line(framebuffer, x0, y0, x2, y2, color);
	draw_line(framebuffer, x1, y1, x2, y2, color);
}

void draw_pixel(unsigned int* framebuffer, int x, int y, unsigned int color){
	if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT) g_Framebuffer[WINDOW_WIDTH * y + x] = color;
}

void quit(){
	//Destroy resources here and quit subsystems like audio, fonts and so on...
	SDL_DestroyWindow(g_Window);
	SDL_DestroyRenderer(g_Renderer);
	SDL_Quit();
}