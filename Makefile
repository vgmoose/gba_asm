
ARCH=$(shell uname -m)

define cecho
      @tput setaf 3
      @echo "[ $1 ]"
      @tput sgr0
endef

build:	1up.c homebrew.s draw.c draw.h dummy.c

ifneq (,$(findstring arm, $(ARCH)))
		$(call cecho, "making ARM binary")
		as -g homebrew.s -o homebrew.o -mthumb-interwork
		gcc -g -o 1up -lpthread -std=gnu11 draw.c draw.h homebrew.o 1up.c  -lSDLmain -lSDL -mthumb-interwork -lm
		$(call cecho, "built 1up")
	
else
		$(call cecho, "cross-compiling GBA rom")
		arm-none-eabi-gcc -mthumb-interwork -specs=gba.specs dummy.c homebrew.s -o homebrew.o
		arm-none-eabi-objcopy -O binary homebrew.o homebrew.gba
		$(call cecho, "built homebrew.gba")
endif

clean:
	rm *.o *.gba 1up 
