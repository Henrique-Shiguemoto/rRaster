#include <iostream>

#define SDL_MAIN_HANDLED
#include "SDL.h"

#include "main.h"

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

	draw_background(g_Framebuffer, 0x00FF0000);
	draw_AABB(g_Framebuffer, 100, 100, 200, 200, 0x000000FF);
	draw_AABB(g_Framebuffer, 150, 150, 250, 250, 0x00000000);

	render_end();
}

void draw_background(unsigned int* framebuffer, unsigned int color){
	for(int i = 0; i < WINDOW_HEIGHT; i++)
		for (int j = 0; j < WINDOW_WIDTH; ++j)
			draw_pixel(g_Framebuffer, j, i, color);
}

void draw_AABB(unsigned int* framebuffer, int minX, int minY, int maxX, int maxY, unsigned int color){
	if(minX < 0) minX = 0;
	if(minY < 0) minY = 0;
	if(maxX < 0) maxX = 0;
	if(maxY < 0) maxY = 0;
	if(minX > WINDOW_WIDTH)  minX = WINDOW_WIDTH - 1;
	if(minY > WINDOW_HEIGHT) minY = WINDOW_HEIGHT - 1;
	if(maxX > WINDOW_HEIGHT) maxX = WINDOW_HEIGHT - 1;
	if(maxY > WINDOW_WIDTH)  maxY = WINDOW_WIDTH - 1;

	for (int i = minY; i < maxY; ++i){
		for (int j = minX; j < maxX; ++j){
			draw_pixel(g_Framebuffer, j, i, color);
		}
	}
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