#include "draw.h"
#include <SDL2/SDL.h>

SDL_Renderer* sdl_screen;
SDL_Texture* texture;
uint16_t screen[screen_x * screen_y]; // bgr555

void screen_clear()
{
        SDL_RenderClear(sdl_screen);
}

void screen_flip()
{
	SDL_UpdateTexture(texture, NULL, screen, 2 * screen_x);
	SDL_RenderCopy(sdl_screen, texture, NULL, NULL);
        SDL_RenderPresent(sdl_screen);
}

char screen_init(const char * title)
{
	// initialize SDL
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("1up", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_x, screen_y, 0);

	if (window == NULL)
	{
		printf("GUI: %s\n",SDL_GetError());
		return 0;
	}

	sdl_screen = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// ensure properly initialized
	if (sdl_screen == NULL)
	{
		printf("GUI: Could not connect to display! %s\n", SDL_GetError());
		return 0;
	}

	texture = SDL_CreateTexture(sdl_screen,
                                        SDL_PIXELFORMAT_BGR555,
                                        SDL_TEXTUREACCESS_STREAMING,
                                        screen_x, screen_y);

	// setup the screen to draw to later
	//screen = (uint16_t *) texture->pixels;

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

void draw_pixel(int x, int y, uint16_t b)
{
	screen[ (y * screen_x + x) ] = b;
}
