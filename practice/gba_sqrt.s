.text

.global main

main:
	mov	r0, #4
	swi	0x80000
	mov	r1, r0
	ldr	r0, =format
	bl	printf
	mov	r0, #0
	bl	exit

.data

format:
	.asciz "%d\n"
