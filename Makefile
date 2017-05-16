
ARCH=$(shell uname -m)

define cecho
      @tput setaf 3
      @echo "[ $1 ]"
      @tput sgr0
endef

build:	

ifneq (,$(findstring arm, $(ARCH)))
		$(call cecho, "making ARM binary")
		# as -g homebrew.s -o homebrew.o -mthumb-interwork
		gcc -g homebrew/* 1up/* -o 1up.out -lpthread -std=gnu11 -lSDLmain -lSDL -mthumb-interwork -lm
		$(call cecho, "built 1up.out")
	
else
		$(call cecho, "cross-compiling GBA rom")
		arm-none-eabi-gcc -mthumb-interwork -specs=gba.specs homebrew/* 0up/* -o homebrew.o
		arm-none-eabi-objcopy -O binary *.o homebrew.gba
		$(call cecho, "built homebrew.gba")
endif

clean:
	rm *.o *.gba *.out
