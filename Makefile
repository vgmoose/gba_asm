build:	1up.c homebrew.s
	
	# make ARM linux binary 
	as homebrew.s -o homebrew.o
	gcc -o 1up -lpthread -std=gnu11 -Wimplicit-function-declaration draw.c draw.h -Wimplicit-function-declaration homebrew.o 1up.c  -lSDLmain -lSDL 
