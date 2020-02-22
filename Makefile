
ARCH=$(shell uname -m)

define cecho
      @tput setaf 3
      @echo "[ $1 ]"
      @tput sgr0
endef

build:	

ifneq (,$(findstring arm, $(ARCH)))
		$(call cecho, "making ARM binaries")
		gcc -g 1up/* -o 1up_driver -std=gnu11 -lSDL2main -lSDL2 -mthumb-interwork -lm -ldl -lrt
		gcc -g homebrew/*.[cs] 1up/shared.c -o gba.out -mthumb-interwork -ldl -lrt -Wl,-einit_wrapper
		$(call cecho, "built 1up_driver and gba.out")
	
else
		$(call cecho, "cross-compiling GBA rom")
		$(DEVKITARM)/bin/arm-none-eabi-gcc -mthumb-interwork -specs=gba.specs homebrew/*.[cs] -o homebrew.o
		$(DEVKITARM)/bin/arm-none-eabi-objcopy -O binary *.o homebrew.gba
		$(call cecho, "built homebrew.gba")
endif

clean:
	rm *.o *.gba *.out
