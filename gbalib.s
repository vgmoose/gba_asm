.text

.global jump_table

jump_table:
	# subtract for now, even though it should be division
	subs	r0, r0, #1
	mov	pc, r14
