#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "draw.h"


pthread_t graphics_thread;

void * graphics_threadf()
{
	// initialize the SDL screen
	char res = screen_init("GBA");

	// return if failed to initialize, ending thread
	if (!res)
		return NULL;

	// clear the display
 	screen_clear();
	
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

// 	screen_end();

	return NULL;
}

int init()
{
	// map vitual memory for screen mode
	mmap((void*)0x4000000, 0x2, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_SHARED | MAP_FIXED, -1, 0);

	// map virtual memory for screen buffer
	mmap((void*)0x6000000, 0x9600*3, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_SHARED | MAP_FIXED, -1, 0);

	// map virtual memory for swi hooks
	mmap((void*)0x80000, 0x100, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_SHARED | MAP_FIXED, -1, 0);

	// create the SDL thread to display the image
	// TODO: races the asm code
	pthread_create(&graphics_thread, NULL, graphics_threadf, NULL);
}

int deinit()
{
	// create a screen.bmp file
	int fd = fileno(fopen("screen.bmp", "wb"));

	// header for 16-bit bmp file

        char header [] = { 0x42, 0x4D,          // "BM"
                        0x70, 0xB6, 0x0, 0x0,  // total size (header+data)
                        0, 0, 0, 0,             // reserved data
                        54, 0, 0, 0,            // number bytes in header
                        40, 0, 0, 0,            // BITMAPINFOHEADER = 40 bytes
                        240, 0, 0, 0,           // width in pixels
                        0x100 - 160, 0xFF, 0xFF, 0xFF,           // height in pixels
                        1, 0,                   // number of color planes
                        16, 0,                  // bits per pixel
                        0, 0, 0, 0,             // no compression
                        0x00, 0xC2, 0x01, 0x00, // size of pixel data
                        0x13, 0x0b, 0, 0,       // horizontal resolution
                        0x13, 0x0b, 0, 0,       // vertical resolution
                        0, 0, 0, 0,             // number of colors (0=all)
                        0, 0, 0, 0};            // important colors, (0=all)

	// write bitmap header to disk
	write(fd, header, 54);

	// write image data to disk from screen buffer
	write(fd, (void *) 0x6000000, 0x9600*2);		

	printf("Wrote screenshot to screen.bmp\n");

	// exit syscall to stop gracefully
	while(1);
}

// this method will perform a fake software interrupt depending on
// the value that is passed into it
int fake_swi(int arg1, int arg2, int arg3, int swi)
{
	if (swi == 0x80000)
		return sqrt(arg1);

	return 0;
}
