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
	
	g_Rasterizer->raster_background(RRASTER_COLOR_DARK_BLUE);

	int health_bar_width = 150;
	int health_bar_height = 25;
	g_Rasterizer->raster_AABB_filled(25, WINDOW_HEIGHT - 15, 25 + health_bar_width, WINDOW_HEIGHT - 15 - health_bar_height, RRASTER_COLOR_BLACK);
	g_Rasterizer->raster_AABB_filled(28, WINDOW_HEIGHT - 18, 22 + health_bar_width, WINDOW_HEIGHT - 12 - health_bar_height, RRASTER_COLOR_LIGHT_GREEN);
	g_Rasterizer->raster_text("Health: 100%", 32, WINDOW_HEIGHT - 32, RRASTER_COLOR_BLACK);

	g_Rasterizer->raster_image("assets/imgs/greenEllipses.png", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 40);
	g_Rasterizer->raster_image("assets/imgs/greenEllipses.png", WINDOW_WIDTH / 2 + 10, WINDOW_HEIGHT / 2 + 80);
	g_Rasterizer->raster_image("assets/imgs/greenEllipses.png", WINDOW_WIDTH / 2 + 16, WINDOW_HEIGHT / 2 + 120);
	g_Rasterizer->raster_image("assets/imgs/purpleSpaceship.png", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 180);

	g_Rasterizer->raster_circle((WINDOW_WIDTH / 2) - 120, 40, 15.0f, RRASTER_COLOR_YELLOW);
	g_Rasterizer->raster_circle((WINDOW_WIDTH / 2) + 20, 70, 35.0f, RRASTER_COLOR_YELLOW);
	g_Rasterizer->raster_circle((WINDOW_WIDTH / 2) + 170, 120, 27.0f, RRASTER_COLOR_YELLOW);
	g_Rasterizer->raster_circle((WINDOW_WIDTH / 2) - 220, 270, 5.0f, RRASTER_COLOR_YELLOW);
	g_Rasterizer->raster_circle((WINDOW_WIDTH / 2) - 110, 220, 18.0f, RRASTER_COLOR_YELLOW);

	if(g_DebugInfoIsShowing){
		char debug_string[256] = "";
		snprintf(debug_string, 256, "FPS: %lf", 1000.0 / g_perf.delta_time);
		g_Rasterizer->raster_text(debug_string, 10, 10, RRASTER_COLOR_WHITE);
		
		snprintf(debug_string, 256, "Frame Count: %lu", g_perf.framecount);
		g_Rasterizer->raster_text(debug_string, 10, 20, RRASTER_COLOR_WHITE);
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