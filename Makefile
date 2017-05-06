
ARCH=$(shell uname -m)
build:	1up.c homebrew.s

ifneq (,$(findstring arm, $(ARCH)))
		# making ARM linux binary 
		as homebrew.s -o homebrew.o
		gcc -o 1up -lpthread -std=gnu11 -Wimplicit-function-declaration draw.c draw.h -Wimplicit-function-declaration homebrew.o 1up.c  -lSDLmain -lSDL 
	
else
		# cross-compiling GBA rom
		arm-none-eabi-gcc -mthumb-interwork -specs=gba.specs dummy.c homebrew.s -o homebrew.o
		arm-none-eabi-objcopy -O binary homebrew.o homebrew.gba;
endif

