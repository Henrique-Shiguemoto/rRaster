#include <iostream> // for logging errors mostly, not really necessary

#define SDL_MAIN_HANDLED
#include "SDL.h"

#include "main.h"

#include "rRaster.h"

bool g_GameIsRunning = false;
SDL_Window* g_Window = nullptr;

rRaster* g_Rasterizer = nullptr;

int main(){
	if(!init()) return 1;
	create_window();
	
	g_Rasterizer = new rRaster(g_Window, WINDOW_WIDTH, WINDOW_HEIGHT);

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

void render_graphics(){
	g_Rasterizer->raster_begin();
	g_Rasterizer->raster_background(RRASTER_COLOR_DARK_GRAY);
	g_Rasterizer->raster_pixel(0, 0, RRASTER_COLOR_MAGENTA);
	g_Rasterizer->raster_AABB(200, 210, 280, 270, RRASTER_COLOR_LIGHT_LIGHT_YELLOW);
	g_Rasterizer->raster_AABB_filled(300, 310, 320, 330, RRASTER_COLOR_DARK_RED);
	g_Rasterizer->raster_line(70, 120, 75, 478, RRASTER_COLOR_LIGHT_CYAN);
	g_Rasterizer->raster_circle(WINDOW_WIDTH - 70, WINDOW_HEIGHT - 70, 33.0f, RRASTER_COLOR_GREEN);
	g_Rasterizer->raster_triangle(120, 20, 32, 105, 495, 130, RRASTER_COLOR_BLACK);
	g_Rasterizer->raster_end();
}

void quit(){
	SDL_DestroyWindow(g_Window);
	SDL_Quit();
}