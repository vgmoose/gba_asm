build:	1up.c homebrew.s
	
	# make ARM linux binary 
	as homebrew.s -o homebrew.o
	gcc homebrew.o 1up.c -o 1up
