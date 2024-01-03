#include <iostream>

#define SDL_MAIN_HANDLED
#include "SDL.h"

#include "main.h"

bool g_GameIsRunning = false;
SDL_Window* g_Window = nullptr;
SDL_Renderer* g_Renderer = nullptr;

bool g_RButtonWasPressed = false;
bool g_GButtonWasPressed = false;
bool g_BButtonWasPressed = false;

unsigned char g_Red = 0;
unsigned char g_Green = 0;
unsigned char g_Blue = 0;

int main(){
	if(!init()) return 1;
	create_window();
	
	while(g_GameIsRunning){
		handle_input();
		simulate_world();
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
	
	//you can add other SDL modules here, like SDL_Audio, TTF_Font, SDL_Image, etc
	
	g_GameIsRunning = true;
	
	return true;
}

void create_window(){
	if((g_Window = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, SDL_WINDOW_SHOWN))) 
		std::cout << SDL_GetError() << std::endl;
	if((g_Renderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED)))
		std::cout << SDL_GetError() << std::endl;
}

void handle_input(){
	SDL_Event e;
	while(SDL_PollEvent(&e)){
		const unsigned char* keyboardState = SDL_GetKeyboardState(NULL);
		
		//keyboard stuff
		g_GameIsRunning = !keyboardState[SDL_SCANCODE_ESCAPE];
		if(keyboardState[SDL_SCANCODE_R]){
			g_RButtonWasPressed = true;
			std::cout << "R = " << (int)g_Red << "\n";
		}
		if(keyboardState[SDL_SCANCODE_G]){
			g_GButtonWasPressed = true;
			std::cout << "G = " << (int)g_Green << "\n";
		}
		if(keyboardState[SDL_SCANCODE_B]){
			g_BButtonWasPressed = true;
			std::cout << "B = " << (int)g_Blue << "\n";
		}
		
		//mouse stuff
		if(e.type == SDL_MOUSEBUTTONDOWN){
			std::cout << "Pressed mouse button\n";
		}
		if(e.type == SDL_MOUSEMOTION){
			std::cout << "Moved mouse\n";
		}
		if(e.type == SDL_MOUSEWHEEL){
			std::cout << "Moved mouse wheel\n";
		}
		if(e.type == SDL_QUIT){
			g_GameIsRunning = false;
		}
	}
}

void simulate_world(){
	if(g_RButtonWasPressed) g_Red++;
    if(g_GButtonWasPressed) g_Green++;
    if(g_BButtonWasPressed) g_Blue++;
	g_RButtonWasPressed = false;
	g_GButtonWasPressed = false;
	g_BButtonWasPressed = false;
}

void render_graphics(){
	SDL_SetRenderDrawColor(g_Renderer, g_Red, g_Green, g_Blue, 255);
	SDL_RenderClear(g_Renderer);
	SDL_RenderPresent(g_Renderer);
}

void quit(){
	//Destroy resources here and quit subsystems like audio, fonts and so on...
	SDL_DestroyWindow(g_Window);
	SDL_DestroyRenderer(g_Renderer);
	SDL_Quit();
}