
ARCH=$(shell uname -m)

define cecho
      @tput setaf 3
      @echo "[ $1 ]"
      @tput sgr0
endef

build:	

ifneq (,$(findstring arm, $(ARCH)))
		$(call cecho, "making ARM binary")
		gcc -g homebrew/*.[cs] 1up/* -o gba.out -lpthread -std=gnu11 -lSDLmain -lSDL -mthumb-interwork -lm -Wl,-einit
		python swi_convert.py gba.out
		$(call cecho, "built gba.out")
	
else
		$(call cecho, "cross-compiling GBA rom")
		$(DEVKITARM)/bin/arm-none-eabi-gcc -mthumb-interwork -specs=gba.specs homebrew/*.[cs] -o homebrew.o
		$(DEVKITARM)/bin/arm-none-eabi-objcopy -O binary *.o homebrew.gba
		$(call cecho, "built homebrew.gba")
endif

clean:
	rm *.o *.gba *.out
