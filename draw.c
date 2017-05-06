#include "draw.h"
#include <SDL/SDL.h>

uint8_t * screen;
SDL_Surface *sdl_screen;

void screen_clear()
{
	SDL_FillRect(sdl_screen, NULL, 0xFFFFFF);
}

void screen_flip()
{
	SDL_Flip(sdl_screen);
}

void screen_init(const char * title)
{
	printf("init\n");
	SDL_Init(SDL_INIT_VIDEO);
	
	printf("screen\n");
	sdl_screen = SDL_SetVideoMode(screen_x, screen_y, 24, SDL_HWSURFACE | SDL_FULLSCREEN);
	SDL_WM_SetCaption(title, 0);
	
	printf("other\n");
	screen = (uint8_t *)sdl_screen->pixels;
	printf("willclear\n");
	screen_clear();
	printf("idk\n");
	screen_flip();
}

void screen_end()
{
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
}

void draw_pixel(int x, int y, char r, char g, char b)
{
	uint32_t v = (y * screen_x + x) * 3;
	screen[v] = b;
	screen[v+1] = g;
	screen[v+2] = r;
}

void draw_line(int x0, int y0, int x1, int y1, char r, char g, char b)
{
	int dx = abs(x1 - x0), sx = (x0 < x1) ? 1 : -1;
	int dy = abs(y1 - y0), sy = (y0 < y1) ? 1 : -1;
	int err = ((dx > dy) ? dx : -dy) / 2, e2;
 
	for(;;) {
		draw_pixel(x0, y0, r, g, b);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

void draw_rect(int x1, int y1, int x2, int y2, char r, char g, char b)
{
	draw_line(x1, y1, x2, y1, r, g, b);
	draw_line(x2, y1, x2, y2, r, g, b);
	draw_line(x1, y2, x2, y2, r, g, b);
	draw_line(x1, y1, x1, y2, r, g, b);
}

void draw_fill_rect(int x1, int y1, int x2, int y2, char r, char g, char b)
{
	int X1, X2, Y1, Y2, i, j;

	if (x1 < x2){
		X1 = x1;
		X2 = x2;
	}
	else {
		X1 = x2;
		X2 = x1;
	}

	if (y1 < y2){
		Y1 = y1;
		Y2 = y2;
	}
	else {
		Y1 = y2;
		Y2 = y1;
	}
	for (i = X1; i <= X2; i++){
		for (j = Y1; j <= Y2; j++){
			draw_pixel(i, j, r, g, b);
		}
	}
}

void draw_circle(int xCen, int yCen, int radius, char r, char g, char b)
{
	int x = 0;
	int y = radius;
	int p = (5 - radius * 4) / 4;
	draw_circle_circum(xCen, yCen, x, y, r, g, b);
	while (x < y){
		x++;
		if (p < 0){
			p += 2 * x + 1;
		}
		else{
			y--;
			p += 2 * (x - y) + 1;
		}
		draw_circle_circum(xCen, yCen, x, y, r, g, b);
	}
}

void draw_fill_circle(int xCen, int yCen, int radius, char r, char g, char b)
{
	draw_circle(xCen, yCen, radius, r, g, b);
	int x, y;
	for (y = -radius; y <= radius; y++){
		for (x = -radius; x <= radius; x++)
			if (x*x + y*y <= radius*radius + radius * .8f)
				draw_pixel(xCen + x, yCen + y, r, g, b);
	}
}

void draw_circle_circum(int cx, int cy, int x, int y, char r, char g, char b)
{
	if (x == 0){
		draw_pixel(cx, cy + y, r, g, b);
		draw_pixel(cx, cy - y, r, g, b);
		draw_pixel(cx + y, cy, r, g, b);
		draw_pixel(cx - y, cy, r, g, b);
	}
	if (x == y){
		draw_pixel(cx + x, cy + y, r, g, b);
		draw_pixel(cx - x, cy + y, r, g, b);
		draw_pixel(cx + x, cy - y, r, g, b);
		draw_pixel(cx - x, cy - y, r, g, b);
	}
	if (x < y){
		draw_pixel(cx + x, cy + y, r, g, b);
		draw_pixel(cx - x, cy + y, r, g, b);
		draw_pixel(cx + x, cy - y, r, g, b);
		draw_pixel(cx - x, cy - y, r, g, b);
		draw_pixel(cx + y, cy + x, r, g, b);
		draw_pixel(cx - y, cy + x, r, g, b);
		draw_pixel(cx + y, cy - x, r, g, b);
		draw_pixel(cx - y, cy - x, r, g, b);
	}
}
