#include <cstdio> // for logging errors mostly, not really necessary

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "rmathlib.h"
#include "rRaster.h"
#include "main.h"

bool g_AppIsRunning = false;
bool g_debugKeyIsPressed = false;
bool g_DebugInfoIsShowing = true;
bool g_DebugKeyWasPressed = true;
SDL_Window* g_Window = nullptr;
rRaster* g_Rasterizer = nullptr;
perf_data g_perf;

int main(){
	if(!init()) return 1;
	create_window();

	g_Rasterizer = new rRaster(g_Window, WINDOW_WIDTH, WINDOW_HEIGHT);

	g_perf.last_time = SDL_GetTicks64();
	while(g_AppIsRunning){
		handle_input();
		render_graphics();
		update_timers();
	}

	quit();
	return 0;
}

bool init(){
	if(SDL_Init(SDL_INIT_VIDEO)){
		printf("%s\n", SDL_GetError());
		return false;
	}
	
	g_AppIsRunning = true;
	
	return true;
}

void create_window(){
	if((g_Window = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN))) {
		printf("%s\n", SDL_GetError());
	}
}

void handle_input(){
	SDL_Event e;
	while(SDL_PollEvent(&e)){
		const unsigned char* keyboardState = SDL_GetKeyboardState(NULL);
		
		//keyboard stuff
		g_AppIsRunning = !keyboardState[SDL_SCANCODE_ESCAPE];
		g_debugKeyIsPressed = !keyboardState[SDL_SCANCODE_TAB];

		// toggle behavior, that's why it needs another variable (one which checks for the button's state last frame)
		if(g_debugKeyIsPressed && !g_DebugKeyWasPressed){
			g_DebugInfoIsShowing = !g_DebugInfoIsShowing;
		}
		
		//mouse stuff
		if(e.type == SDL_QUIT){
			g_AppIsRunning = false;
		}

		// updating toggle states
		g_DebugKeyWasPressed = g_debugKeyIsPressed;
	}
}

void render_graphics(){
	g_Rasterizer->raster_begin();
	
	g_Rasterizer->raster_background(RRASTER_COLOR_LIGHT_GRAY);

	if(g_DebugInfoIsShowing){
		char debug_string[256] = "";
		snprintf(debug_string, 256, "FPS: %lf", 1000.0 / g_perf.delta_time);
		g_Rasterizer->raster_text(debug_string, 10, 10, RRASTER_COLOR_RED);
		
		snprintf(debug_string, 256, "Frame Count: %lu", g_perf.framecount);
		g_Rasterizer->raster_text(debug_string, 10, 20, RRASTER_COLOR_RED);
	}
	
	g_Rasterizer->raster_end();
}

void update_timers(){
	g_perf.framecount++;
	g_perf.current_time = SDL_GetTicks64();
	g_perf.delta_time = g_perf.current_time - g_perf.last_time;
	g_perf.last_time = g_perf.current_time;
}

void quit(){
	SDL_DestroyWindow(g_Window);
	SDL_Quit();
}