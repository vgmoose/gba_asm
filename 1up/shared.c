#include "shared.h"

// forward declare main, as this file is included by our gba binary
int main();

int init_wrapper()
{
  // set up shared mem, and jump to original main function
  init_shared_mem(O_RDWR);
  return main();
}

void vmap(int start, int end, int sharedMem)
{
	// map virtual memory between specified addresses
	mmap((void*)start, end-start, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_FIXED, sharedMem, start);
}

int init_shared_mem(int flags)
{
  // open named shared memory block
  // TODO: use unique name for the parent process + env variable
  int sm = shm_open("gba_asm", flags, S_IRWXU);
  if (sm == -1)
  {
      printf("Can't open shared memory block/file\n");
      return -3;
  }

  // TODO: this uses way more memory than needed (due to gaps),
  // can be split up into individual files for each streak
  int res = ftruncate(sm, 0x0F000000);
  if (res == -1)
  {
      printf("Couldn't size shared memory correctly\n");
      return -4;
  }

	// info is from:
	// https://www.reinterpretcast.com/writing-a-game-boy-advance-game

	// 16 KB System ROM (executable, but not readable)
	vmap(0x00000000, 0x00003FFF, sm);

	// 256 KB EWRAM (general purpose RAM external to the CPU)
	vmap(0x02000000, 0x02030000, sm);

	// 32 KB IWRAM (general purpose RAM internal to the CPU)
	vmap(0x03000000, 0x03007FFF, sm);

	// I/O Registers	
	vmap(0x04000000, 0x040003FF, sm);

	// 1 KB Colour Palette RAM
	vmap(0x05000000, 0x050003FF, sm);

	// 96 KB VRAM (Video RAM)
	vmap(0x06000000, 0x06017FFF, sm);

	// 1 KB OAM RAM (Object Attribute Memory)
	vmap(0x07000000, 0x070003FF, sm);

	// Game Pak ROM (0 to 32 MB)
	vmap(0x08000000, 0x09E84800, sm);

	// Game Pak RAM
	vmap(0x0E000000, 0x0F000000, sm);
}