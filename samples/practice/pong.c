#include "../1up/1up.h"

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 160

#define MEM_IO   0x04000000
#define MEM_PAL  0x05000000
#define MEM_VRAM 0x06000000
#define MEM_OAM  0x07000000

#define REG_DISPLAY        (*((volatile uint32 *)(MEM_IO)))
#define REG_DISPLAY_VCOUNT (*((volatile uint32 *)(MEM_IO + 0x0006)))
#define REG_KEY_INPUT      (*((volatile uint32 *)(MEM_IO + 0x0130)))

#define KEY_UP   0x0040
#define KEY_DOWN 0x0080
#define KEY_ANY  0x03FF

#define OBJECT_ATTR0_Y_MASK 0x0FF
#define OBJECT_ATTR1_X_MASK 0x1FF

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef uint16 rgb15;
typedef struct obj_attrs {
	uint16 attr0;
	uint16 attr1;
	uint16 attr2;
	uint16 pad;
} __attribute__((packed, aligned(4))) obj_attrs;
typedef uint32    tile_4bpp[8];
typedef tile_4bpp tile_block[512];

#define oam_mem            ((volatile obj_attrs *)MEM_OAM)
#define tile_mem           ((volatile tile_block *)MEM_VRAM)
#define object_palette_mem ((volatile rgb15 *)(MEM_PAL + 0x200))

// Form a 16-bit BGR GBA colour from three component values
static inline rgb15 RGB15(int r, int g, int b)
{
	return r | (g << 5) | (b << 10);
}

// Set the position of an object to specified x and y coordinates
static inline void set_object_position(volatile obj_attrs *object, int x,
                                       int y)
{
	object->attr0 = (object->attr0 & ~OBJECT_ATTR0_Y_MASK) |
	                (y & OBJECT_ATTR0_Y_MASK);
	object->attr1 = (object->attr1 & ~OBJECT_ATTR1_X_MASK) |
	                (x & OBJECT_ATTR1_X_MASK);
}

// Clamp 'value' in the range 'min' to 'max' (inclusive)
static inline int clamp(int value, int min, int max)
{
	return (value < min ? min
	                    : (value > max ? max : value));
}

int main(void)
{
	init();

	// Write the tiles for our sprites into the fourth tile block in VRAM.
	// Four tiles for an 8x32 paddle sprite, and one tile for an 8x8 ball
	// sprite. Using 4bpp, 0x1111 is four pixels of colour index 1, and
	// 0x2222 is four pixels of colour index 2.
	//
	// NOTE: We're using our own memory writing code here to avoid the
	// byte-granular writes that something like 'memset' might make (GBA
	// VRAM doesn't support byte-granular writes).
	volatile uint16 *paddle_tile_mem = (uint16 *)tile_mem[4][1];
	volatile uint16 *ball_tile_mem   = (uint16 *)tile_mem[4][5];
	for (int i = 0; i < 4 * (sizeof(tile_4bpp) / 2); ++i)
		paddle_tile_mem[i] = 0x1111; // 0b_0001_0001_0001_0001
	for (int i = 0; i < (sizeof(tile_4bpp) / 2); ++i)
		ball_tile_mem[i] = 0x2222;   // 0b_0002_0002_0002_0002

	// Write the colour palette for our sprites into the first palette of
	// 16 colours in colour palette memory (this palette has index 0)
	object_palette_mem[1] = RGB15(0x1F, 0x1F, 0x1F); // White
	object_palette_mem[2] = RGB15(0x1F, 0x00, 0x1F); // Magenta

	// Create our sprites by writing their object attributes into OAM
	// memory
	volatile obj_attrs *paddle_attrs = &oam_mem[0];
	paddle_attrs->attr0 = 0x8000; // 4bpp tiles, TALL shape
	paddle_attrs->attr1 = 0x4000; // 8x32 size when using the TALL shape
	paddle_attrs->attr2 = 1;      // Start at the first tile in tile
	                              // block four, use color palette zero
	volatile obj_attrs *ball_attrs = &oam_mem[1];
	ball_attrs->attr0 = 0; // 4bpp tiles, SQUARE shape
	ball_attrs->attr1 = 0; // 8x8 size when using the SQUARE shape
	ball_attrs->attr2 = 5; // Start at the fifth tile in tile block four,
	                       // use color palette zero

	// Initialize variables to keep track of the state of the paddle and
	// ball, and set their initial positions (by modifying their
	// attributes in OAM)
	const int player_width = 8,
	          player_height = 32;
	const int ball_width = 8,
	          ball_height = 8;
	int player_velocity = 2;
	int ball_velocity_x = 2,
	    ball_velocity_y = 1;
	int player_x = 5,
	    player_y = 96;
	int ball_x = 22,
	    ball_y = 96;
	set_object_position(paddle_attrs, player_x, player_y);
	set_object_position(ball_attrs, ball_x, ball_y);

	// Set the display parameters to enable objects, and use a 1D
	// object->tile mapping
	REG_DISPLAY = 0x1000 | 0x0040;

	// The main game loop
	uint32 key_states = 0;
	while (1) {
		// Skip past the rest of any current V-Blank, then skip past
		// the V-Draw
		while(REG_DISPLAY_VCOUNT >= 160);
		while(REG_DISPLAY_VCOUNT <  160);

		// Get current key states (REG_KEY_INPUT stores the states
		// inverted)
		key_states = ~REG_KEY_INPUT & KEY_ANY;

		// Note that our physics update is tied to the framerate,
		// which isn't generally speaking a good idea. Also, this is
		// really terrible physics and collision handling code.
		int player_max_clamp_y = SCREEN_HEIGHT - player_height;
		if (key_states & KEY_UP)
			player_y = clamp(player_y - player_velocity, 0,
			                 player_max_clamp_y);
		if (key_states & KEY_DOWN)
			player_y = clamp(player_y + player_velocity, 0,
			                 player_max_clamp_y);
		if (key_states & KEY_UP || key_states & KEY_DOWN)
			set_object_position(paddle_attrs, player_x, player_y);

		int ball_max_clamp_x = SCREEN_WIDTH  - ball_width,
		    ball_max_clamp_y = SCREEN_HEIGHT - ball_height;
		if ((ball_x >= player_x &&
		     ball_x <= player_x + player_width) &&
		    (ball_y >= player_y &&
		     ball_y <= player_y + player_height)) {
			ball_x = player_x + player_width;
			ball_velocity_x = -ball_velocity_x;
		} else {
			if (ball_x == 0 || ball_x == ball_max_clamp_x)
				ball_velocity_x = -ball_velocity_x;
			if (ball_y == 0 || ball_y == ball_max_clamp_y)
				ball_velocity_y = -ball_velocity_y;
		}

		ball_x = clamp(ball_x + ball_velocity_x, 0, ball_max_clamp_x);
		ball_y = clamp(ball_y + ball_velocity_y, 0, ball_max_clamp_y);
		set_object_position(ball_attrs, ball_x, ball_y);
	}

	deinit();

	return 0;
}
