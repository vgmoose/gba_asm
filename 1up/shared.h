#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

int init_shared_mem(int flags);
void vmap(int start, int end, int sharedMem);