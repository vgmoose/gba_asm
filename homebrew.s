.text

.global main

main:
	// jump to initialization code
	bl init

	// set gba display mode
 	mov	r0, #0x4000000
 	mov	r1, #0x400
 	adds	r1, r1, #3
 	str	r1, [r0]

	// offset of screenbuffer in memory
	mov	r0, #0x6000000

	// the color red
	mov	r1, #0xFF

	// set a counter for the number of pixels on screen
	mov	r2, #0x9600

draw:
	// set them all to red
	strh	r1, [r0], #3
	subs	r2, r2, #1

	// continue if countdown not done
	bne	draw

	// jump to deinitialization code
	bl	deinit
