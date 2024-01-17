#include "rRaster.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "../assets/fonts/custom_font.cpp"

rRaster::rRaster(SDL_Window* window, int context_width, int context_height){
	this->raster_width = context_width;
	this->raster_height = context_height;
	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!this->renderer){
		std::cout << SDL_GetError() << std::endl;
	}

	this->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING | SDL_TEXTUREACCESS_TARGET, this->raster_width, this->raster_height);
	if(!this->texture){
		std::cout << SDL_GetError() << std::endl;
	}

	this->framebuffer = new unsigned int [this->raster_width * this->raster_height]{}; // {} makes sure it's initialized to 0
}

rRaster::~rRaster(){
	for (auto& _pair: this->surface_hashtable) {
	    SDL_FreeSurface(_pair.second);
	}
	SDL_DestroyTexture(this->texture);
	SDL_DestroyRenderer(this->renderer);
	delete[] this->framebuffer;
}

void rRaster::raster_background(unsigned int color){
	for(int i = 0; i < this->raster_height; i++)
		for (int j = 0; j < this->raster_width; ++j)
			raster_pixel(j, i, color);
}

void rRaster::raster_AABB(int minX, int minY, int maxX, int maxY, unsigned int color){
	raster_line(minX, minY, maxX, minY, color);
	raster_line(maxX, minY, maxX, maxY, color);
	raster_line(maxX, maxY, minX, maxY, color);
	raster_line(minX, maxY, minX, minY, color);
}

void rRaster::raster_AABB_filled(int minX, int minY, int maxX, int maxY, unsigned int color){
	for (int i = minY; i < maxY; ++i)
		for (int j = minX; j < maxX; ++j)
			raster_pixel(j, i, color);
}

void rRaster::raster_image(std::string filepath, int x, int y){
	// checking if this filepath is already cached, this right here is really the way to check for elements in a hashtable, really...
	auto item = this->surface_hashtable.find(filepath);
	if(item == this->surface_hashtable.end()){
		if(!this->add_surface_to_surface_hashtable(filepath)){
			return;
		}
	}

	SDL_Surface* cached_surface = this->surface_hashtable[filepath];
	int w = cached_surface->w;
	int h = cached_surface->h;

	for (int _y = 0; _y < h; ++_y) {
		for (int _x = 0; _x < w; ++_x) {
			unsigned int color = *((unsigned int*)cached_surface->pixels + (_y * w) + _x);
			unsigned int a = (color & 0xFF000000) >> 24; // getting the alpha channel data only
			if(a != 0){
				// if alpha is zero, we'll assume the pixel is transparent
				this->raster_pixel(x + _x, y + _y, color);
			}
		}
	}
}

bool rRaster::add_surface_to_surface_hashtable(std::string filepath){
	int width, height, nrComponents;
    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrComponents, 0);
    
    if(data){
    	// We can't free the data returned from stbi_load because SDL_CreateRGBSurfaceFrom just copies 
    	//			the pointer from the first parameter (data) to the surface's pixels member, which means
    	//			if we use stbi_image_free, we'll also be freeing the surface's pixels data
    	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(data, width, height, 32, width * 4, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    	this->surface_hashtable[filepath] = surface;
    	return true;
    }else{
    	stbi_image_free(data);
    	return false;
    }
}

void rRaster::raster_line(int x0, int y0, int x1, int y1, unsigned int color){
	int dx = x1 - x0;
	int dy = y1 - y0;
	int cx = (dx > 0) ? 1 : -1;
	int cy = (dy > 0) ? 1 : -1;

	// vertical lines
	if(dx == 0){
		if(dy == 0) {
			raster_pixel(x0, y0, color);
			return;
		}
		
		// we're deciding if we go up or down
		// 		- if up,   then y1 is smaller than y0 (dy < 0), so we have to decrement y (cy = -1)
		// 		- if down, then y1 is bigger  than y0 (dy > 0), so we have to increment y (cy = +1)
		// also, we're multiplying the condition inequation by cy because we need to flip it when cy = -1
		for (int y = y0; cy * y < cy * y1; y += cy) raster_pixel(x0, y, color);
	}else if (RRASTER_ABS(dy / dx) < 1){
		// bresenham
		float error = 0.0f;
		float threshold = 0.5f;
		float m = (float)dy / (float)dx; // rise over run

		int start_x = x0;
		int start_y = y0;
		int dest_x = x1;

		while(cx * start_x <= cx * dest_x){
			raster_pixel(start_x, start_y, color);
			start_x += cx;
			error += RRASTER_ABS(m);
			if(error >= threshold){
				start_y += cy;
				threshold += 1.0f;
			}
		}
	}else{
		// bresenham
		float error = 0.0f;
		float threshold = 0.5f;
		float m = (float)dx / (float)dy; // run over rise (this changed)

		int start_x = x0;
		int start_y = y0;
		int dest_y = y1; // this changed

		while(cy * start_y <= cy * dest_y){ //this changed
			raster_pixel(start_x, start_y, color);
			start_y += cy; // this changed
			error += RRASTER_ABS(m);
			if(error >= threshold){
				start_x += cx; // this changed
				threshold += 1.0f;
			}
		}
	}
}

void rRaster::raster_circle_filled(int cX, int cY, float radius, unsigned int color){
	int dx = 2 * radius;
	int dy = dx;

	// translating to (0, 0) then translating to (cX, cY)
	for (int y = -dy; y < dy; ++y){
		for (int x = -dx; x < dx; ++x){
			if(y * y + x * x <= radius * radius){
				this->raster_pixel(x + cX, y + cY, color);
			}
		}
	}
}

void rRaster::raster_circle(int cX, int cY, float radius, unsigned int color){
	// bresenham
	int start_x = 0;
	int start_y = -(int)radius;
	float radius2 = radius * radius;

	while(start_x <= -start_y){
		raster_pixel( start_x + cX,  start_y + cY, color);
		raster_pixel( start_x + cX, -start_y + cY, color);
		raster_pixel(-start_x + cX,  start_y + cY, color);
		raster_pixel(-start_x + cX, -start_y + cY, color);
		raster_pixel( start_y + cX,  start_x + cY, color);
		raster_pixel(-start_y + cX,  start_x + cY, color);
		raster_pixel( start_y + cX, -start_x + cY, color);
		raster_pixel(-start_y + cX, -start_x + cY, color);

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

// This function assumes a monospaced bitmap font from the file custom_font.c
void rRaster::raster_text(std::string text, int x, int y, unsigned int color){
	int row_offset = 0;
	int col_offset = 0;

	unsigned int c_index = 0;
	while(c_index < text.size()){
		char current_char = text.at(c_index);

		this->calculate_row_and_column_offset_to_custom_font(current_char, &row_offset, &col_offset);

		for (int _y = 0; _y < CUSTOM_FONT_CHAR_HEIGHT; ++_y) {
			for (int _x = 0; _x < CUSTOM_FONT_CHAR_WIDTH; ++_x) {
				unsigned int p_color = custom_small_font[((row_offset * CUSTOM_FONT_CHAR_HEIGHT + _y) * CUSTOM_FONT_WIDTH_IN_PIXELS) + ((col_offset * CUSTOM_FONT_CHAR_WIDTH) + _x)]; // for now
				unsigned int a = (p_color & 0xFF000000) >> 24; // getting the alpha channel data only
				if(a != 0){
					this->raster_pixel((x + (c_index * 6)) + _x, y + _y, color);
				}
			}
		}

		c_index++;
	}
}

void rRaster::calculate_row_and_column_offset_to_custom_font(char c, int* r_offset, int* c_offset){
	if('a' <= c && c <= 'z'){
		*r_offset = 0;
		*c_offset = static_cast<int>(c - 'a');
	}else if('A' <= c && c <= 'Z'){
		*r_offset = 1;
		*c_offset = static_cast<int>(c - 'A');
	}else if('0' <= c && c <= '9'){
		*r_offset = 2;
		*c_offset = static_cast<int>(c - '0');
	}else{
		switch(c){
			case '(': {*r_offset = 2; *c_offset = 10;} break;
			case ')': {*r_offset = 2; *c_offset = 11;} break;
			case ',': {*r_offset = 2; *c_offset = 12;} break;
			case ';': {*r_offset = 2; *c_offset = 13;} break;
			case '.': {*r_offset = 2; *c_offset = 14;} break;
			case ':': {*r_offset = 2; *c_offset = 15;} break;
			case '!': {*r_offset = 2; *c_offset = 16;} break;
			case '?': {*r_offset = 2; *c_offset = 17;} break;
			case '^': {*r_offset = 2; *c_offset = 18;} break;
			case '#': {*r_offset = 2; *c_offset = 19;} break;
			case '[': {*r_offset = 2; *c_offset = 20;} break;
			case ']': {*r_offset = 2; *c_offset = 21;} break;
			case '%': {*r_offset = 2; *c_offset = 22;} break;
			case '/': {*r_offset = 2; *c_offset = 23;} break;
			case '\\': {*r_offset = 2; *c_offset = 24;} break;
			case '$': {*r_offset = 2; *c_offset = 25;} break;
			case '+': {*r_offset = 3; *c_offset = 0;} break;
			case '-': {*r_offset = 3; *c_offset = 1;} break;
			case '*': {*r_offset = 3; *c_offset = 2;} break;
			case '=': {*r_offset = 3; *c_offset = 4;} break;
			case '_': {*r_offset = 3; *c_offset = 5;} break;
			case '\'': {*r_offset = 3; *c_offset = 6;} break;
			case '\"': {*r_offset = 3; *c_offset = 7;} break;
			case '<': {*r_offset = 3; *c_offset = 8;} break;
			case '>': {*r_offset = 3; *c_offset = 9;} break;
			case '&': {*r_offset = 3; *c_offset = 12;} break;
			case '~': {*r_offset = 3; *c_offset = 13;} break;
			case ' ': {*r_offset = 3; *c_offset = 14;} break;
			default:  {*r_offset = 3; *c_offset = 15;} break;
		}
	}
}

void rRaster::raster_triangle_filled(int x0, int y0, int x1, int y1, int x2, int y2, unsigned int color){
	int minX = RRASTER_MIN_3(x0, x1, x2);
	int minY = RRASTER_MIN_3(y0, y1, y2);
	int maxX = RRASTER_MAX_3(x0, x1, x2);
	int maxY = RRASTER_MAX_3(y0, y1, y2);

	for (int y = minY; y <= maxY; ++y){
		for (int x = minX; x <= maxX; ++x){
			if(this->point_inside_triangle(x, y, x0, y0, x1, y1, x2, y2)){
				this->raster_pixel(x, y, color);
			}
		}
	}

}

void rRaster::raster_triangle(int x0, int y0, int x1, int y1, int x2, int y2, unsigned int color){
	raster_line(x0, y0, x1, y1, color);
	raster_line(x0, y0, x2, y2, color);
	raster_line(x1, y1, x2, y2, color);
}

void rRaster::raster_pixel(int x, int y, unsigned int color){
	if (x >= 0 && x < this->raster_width && y >= 0 && y < this->raster_height) this->framebuffer[this->raster_width * y + x] = color;
}

void rRaster::raster_begin(){
	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
	SDL_RenderClear(this->renderer);
}

void rRaster::raster_end(){
	SDL_UpdateTexture(this->texture, NULL, this->framebuffer, (int)(sizeof(unsigned int) * this->raster_width));
	SDL_RenderCopy(this->renderer, this->texture, NULL, NULL);
	SDL_RenderPresent(this->renderer);
}

bool rRaster::point_inside_triangle(int px, int py, int x0, int y0, int x1, int y1, int x2, int y2){
	int v0_v1_x = x1 - x0;
	int v0_v1_y = y1 - y0;
	int v0_p_x  = px - x0;
	int v0_p_y  = py - y0;

	int v1_v2_x = x2 - x1;
	int v1_v2_y = y2 - y1;
	int v1_p_x  = px - x1;
	int v1_p_y  = py - y1;

	int v2_v0_x = x0 - x2;
	int v2_v0_y = y0 - y2;
	int v2_p_x  = px - x2;
	int v2_p_y  = py - y2;

	bool p_is_to_the_right_of_v0_v1 = (v0_v1_x * v0_p_y - v0_p_x * v0_v1_y) <= 0;
	bool p_is_to_the_right_of_v1_v2 = (v1_v2_x * v1_p_y - v1_p_x * v1_v2_y) <= 0;
	bool p_is_to_the_right_of_v2_v0 = (v2_v0_x * v2_p_y - v2_p_x * v2_v0_y) <= 0;

	return (p_is_to_the_right_of_v0_v1) && (p_is_to_the_right_of_v1_v2) && (p_is_to_the_right_of_v2_v0);
}
