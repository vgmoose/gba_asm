.text

.global main

main:
	// set GBA display mode to 3 (bitmap)
	mov	r0, #0x4000000
	mov	r1, #0x400
	adds	r1, r1, #3
	strh	r1, [r0]

	// load the address of pic variable into r0
	ldr	r0, =pic

	// screenbuffer address into r1
	mov	r1, #0x6000000

	// countdown, number of pixels divded by 8
	mov	r10, #0x960

image:
	// load from pic into eight registers
	ldmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}

	// store those into memory
	stmia	r1!, {r2, r3, r4, r5, r6, r7, r8, r9}

	// subtract the counter
	subs	r10, r10, #1

	// go back to loading pixels if not zero
	bne	image

hang:
	bl	hang

.ltorg
pic:
	.incbin	"homebrew/pic.bin"
