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

char screen_init(const char * title)
{
	// initialize SDL
	SDL_Init(SDL_INIT_VIDEO);

	// set the video mode to 16-bit 	
	sdl_screen = SDL_SetVideoMode(screen_x, screen_y, 15, SDL_SWSURFACE);

	// set the title
	SDL_WM_SetCaption(title, 0);

	// ensure properly initialized
	if (sdl_screen == NULL)
	{
		printf("GUI: Could not connect to display!");
		return 0;
	}

	// setup the screen to draw to later
	screen = (uint8_t *)sdl_screen->pixels;

	// clear the whole screen
	screen_clear();

	// commit all previous changes to screen
	screen_flip();

	// successful!
	return 1;
}

void screen_end()
{
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
}

void draw_pixel(int x, int y, char r, char g)
{
	uint32_t v = (y * screen_x + x) * 2;
	screen[v] = r;
	screen[v+1] = g;
}
