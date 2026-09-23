.syntax unified
	.thumb
	.global Audio_ResumePlayer
	.global Func_081c1f00
	.thumb_func
Audio_ResumePlayer:
Func_081c1f00:
	adds	r2, r0, #0
	ldr	r3, [r2, #52]
	ldr	r0, [pc, #12]
	cmp	r3, r0
	bne.n	.L_081c1f12
	ldr	r0, [r2, #4]
	ldr	r1, [pc, #8]
	ands	r0, r1
	str	r0, [r2, #4]
.L_081c1f12:
	bx	lr
	.4byte 0x68736d53
	.4byte 0x7fffffff
