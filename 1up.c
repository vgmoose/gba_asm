#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include "draw.h"

pthread_t graphics_thread;

void * graphics_threadf()
{
	screen_init("GBA Screen");

 	//while(1)
 	//{
 		screen_clear();
 		for (int x = 0; x < 240; x++)
 			for (int y = 0; y < 160; y++)
			{
				char * base_addr = (char *)((0x6000000 + x + (240 * y)*3) );
				printf("%x %x %x\n", *base_addr, *(base_addr+1), *(base_addr+2));
 				draw_pixel(x, y, *base_addr, *(base_addr + 1), *(base_addr + 2));
			}
 		screen_flip();
// 	}
	
// 	screen_end();

	return NULL;
}

int init()
{
	// map vitual memory for screen mode
	mmap((void*)0x4000000, 0x2, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_SHARED | MAP_FIXED, -1, 0);

	// map virtual memory for screen buffer
	mmap((void*)0x6000000, 0x9600*3, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_SHARED | MAP_FIXED, -1, 0);

	pthread_create(&graphics_thread, NULL, graphics_threadf, NULL);
}

int deinit()
{
	// create a screen.ppm file
	FILE * fp = fopen("screen.ppm", "wb");
	int fd = fileno(fp);

	// header for ppm
	fprintf(fp, "P6\n%d %d\n255\n", 240, 160);
	fflush(fp);

	// write pixel data to disk
	write(fd, (void *)0x6000000, 0x9600*3);

	printf("Wrote screenshot to screen.ppm\n");

	// exit syscall to stop gracefully
	while(1);
}
