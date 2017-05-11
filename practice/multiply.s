.text

.global main

main:
	mov	r3, #10
	mov	r2, #5
	mul	r1, r3, r2
	ldr	r0, =string

	bl	printf	
	mov	r0, #0
	bl	exit

.data

string:
	.asciz	"%d\n"
