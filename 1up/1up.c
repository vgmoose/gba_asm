#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include "draw.h"
#include "shared.h"

void m0_draw();
void m3_draw();

int spawn_child_gba_process()
{
  pid_t parent = getpid();
  pid_t pid = fork();

  // fork failed
  if (pid == -1)
    printf("Failed to fork child process.\n");

  // we're the parent, return child pid
  if (pid != 0)
    return pid;

  // down here, we're successfully the child process
  // now it's time to start running the child program
  execv("gba.out", NULL);

  printf("Could not execute gba.out\n");
  exit(-2);
}

void * graphics_threadf()
{
	// initialize the SDL screen
	char res = screen_init("GBA");

	// return if failed to initialize, ending thread
	if (!res)
		return NULL;

	// clear the display
 	screen_clear();

	// the main display loop, constantly polls for pixels in memory and updates them
	// depending on the mode, will take an appropriate action
	while(1)
	{	
		process_input();

		// grab the video mode
		char video_mode = *(char*)(0x4000000);

		// perform the appropriate drawing method based on the video mode
		switch (video_mode)
		{
			case 0x00:
				m0_draw();
				break;
			case 0x03:
				m3_draw();
				break;
			default:
				break;
		}
	}

// 	screen_end();

	return NULL;
}

void m0_draw()
{

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
	
}       

void m3_draw()
{
        // go through all pixels of the display
        for (int y = 0; y < 160; y++)
                for (int x = 0; x < 240; x++)
                {
                        // grab the appropriate address of this pixel from GBA memory
                        uint16_t * base_addr = (uint16_t *)((0x6000000 + x*2 + (240 * y)*2) );

                        // draw this pixel using the two colors at the address
                        draw_pixel(x, y, *base_addr);
                }

        // commit the buffers to display the drawn image
        screen_flip();
}

void intHandler(int dummy)
{
	// quit the program
	exit(0);
}

int main()
{
  // initialize shared memory where the GBA expects it
  init_shared_mem(O_CREAT | O_RDWR);

	// attach the interrupt handler to the SIGINT (Ctrl-C)
	signal(SIGINT, intHandler);

  // we're the parent process, and we'll make a child one
  // to execute a "gba-like" binary (arm, has graphics code, syscalls)
	spawn_child_gba_process();

  // run our graphics intepreter, that looks at the shared VRAM and
  // displays an image in SDL (only supports mode 3)
  // TODO: replace with an emulator's renderer
  // TODO: races the asm code (might be unfixable)
	graphics_threadf();
}

int deinit()
{
	// hang forever so that the SDL window stays up
	while(1);
}

// this method will perform a fake software interrupt depending on
// the value that is passed into it
int fake_swi(int arg1, int arg2, int arg3, int swi)
{

	// Sqrt
	if (swi == 0x80000)
		return sqrt(arg1);

	// ArcTan2	
	if (swi == 0xA0000)
		return atan2(arg1, arg2);

	return sqrt(arg1);
}
