.text

.global main

main:
	mov	r0, #256
	bl	fake_swi
	bl	fake_swi
	bl	fake_swi
	swi	0x80000
	swi	0x80000
	swi	0x80000
	mov	r0, #0
	bl	exit

.data

format:
	.asciz "%d\n"
