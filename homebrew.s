.text

.global main

main:
	mov	r0, #4
	stmdb	sp!, { r3 }
	mov	r3, #0x80000
	bl	fake_swi
	ldmia	sp!, { r3 }
	mov	r1, r0
	ldr	r0, =format
	bl	printf
	mov	r0, #0
	bl	exit

.data

format:
	.asciz "%d\n"
