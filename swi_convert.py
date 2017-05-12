#!/bin/python
# replaces any swi instructions with a jump to the 1up/dummy hooks

# load file full into memory
contents = open("homebrew.s", "r").readlines()

# the string we'll write out later
out = ""

# what we'll swap every swi call with
fake_swi_wrap =   ("\tstmdb	sp!, { r3 }\n"
		   "\tmov	r3, %s\n"
		   "\tbl	fake_swi\n"
		   "\tldmia	sp!, { r3 }\n")

for line in contents:
	if line.lower().strip().startswith("swi"):
		# get index of swi
		index = line.lower().find("swi") + 3
		syscall = line[index:].strip()
		
		# append (#) if needed
		if syscall.startswith("0x"):
			syscall = "#"+syscall

		# insert fake swi wrapper
		line = fake_swi_wrap % syscall
	out += line

# write it all to the file we opened
open("homebrew.s", "w").write(out)
