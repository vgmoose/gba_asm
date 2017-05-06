#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

int init()
{
	// map vitual memory for screen mode
	mmap((void*)0x4000000, 0x2, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_SHARED | MAP_FIXED, -1, 0);

	// map virtual memory for screen buffer
	mmap((void*)0x6000000, 0x9600*3, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_SHARED | MAP_FIXED, -1, 0);
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
	exit(0);
}
