#ifndef MAIN_H
#define MAIN_H

#define GAME_NAME 							"rRaster"
#define WINDOW_WIDTH  						600
#define WINDOW_HEIGHT 						600
#define DESIRED_FPS 						60
#define FRAME_COUNT_TO_UPDATE_FPS 			90

// time is in miliseconds
typedef struct perf_data{
	double current_time = 0;
	double last_time = 0;
	double delta_time = 0.0f;
	unsigned long int framecount = 0;
} perf_data;

bool init();
void create_window();
void handle_input();
void render_graphics();
void update_timers();
void quit();

#endif
