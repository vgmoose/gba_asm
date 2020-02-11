#include "draw.h"
#include <SDL2/SDL.h>
#include <math.h>

SDL_Renderer* sdl_screen;
SDL_Texture* texture;
uint16_t screen[screen_x * screen_y]; // bgr555

// computer key mappings
SDL_Keycode key_buttons[] = { SDL_SCANCODE_A, SDL_SCANCODE_B, SDL_SCANCODE_BACKSPACE, SDL_SCANCODE_RETURN, SDL_SCANCODE_RIGHT, SDL_SCANCODE_LEFT, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_R, SDL_SCANCODE_L };

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

void process_input()
{
	SDL_Event event;
	if (!SDL_PollEvent(&event))
		return;

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	unsigned int cur = 0;

	// go through all button states and flip their bits based on keyboard state
	for (int x=0; x<10; x++)
		cur |= (!keys[key_buttons[x]]) * ((unsigned int) pow(2, x));

	*((volatile unsigned int*)(0x04000130)) = cur;
	
}
