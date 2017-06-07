#!/bin/python
# replaces any swi instructions with a jump to the 1up/dummy hooks

import os, sys

# get the target filename
try:
	name = sys.argv[1]
except:
	print("Usage: python " + sys.argv[0] + " gba.out")
	exit()

# This method will return the address of the fake swi
# calling function for the given value (eg. 0x8000)
def locate_swi(assembly, val):
	for line in assembly:
		# look for the start of the fake swi function in the assembly
		if line.find("<fake_swi>:") >= 0:
			# found it, the number is going to be the first
			# thing in the line
			return line.split(" ")[0]

# disassemble the binary
os.system("objdump -d %s > .disassembly.s" % name)

# load full binary into memory
binary = open(name, "r").readlines()

# load the full disassembly into memory
assembly = open(".disassembly.s", "r").readlines()

# get the address of the fake_swi function
fake_swi_addr = locate_swi(assembly, 8)

print("address is at %s" % fake_swi_addr)

# write it all to the file we opened
# open(name, "wb").write(binary)

