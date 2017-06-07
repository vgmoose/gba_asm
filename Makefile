
ARCH=$(shell uname -m)

define cecho
      @tput setaf 3
      @echo "[ $1 ]"
      @tput sgr0
endef

build:	

ifneq (,$(findstring arm, $(ARCH)))
		$(call cecho, "making ARM binary")
		gcc -g homebrew/* 1up/* -o gba.out -lpthread -std=gnu11 -lSDLmain -lSDL -mthumb-interwork -lm
		python swi_convert.py gba.out
		$(call cecho, "built gba.out")
	
else
		$(call cecho, "cross-compiling GBA rom")
		arm-none-eabi-gcc -mthumb-interwork -specs=gba.specs homebrew/* 0up/* -o homebrew.o
		arm-none-eabi-objcopy -O binary *.o homebrew.gba
		$(call cecho, "built homebrew.gba")
endif

clean:
	rm *.o *.gba *.out
