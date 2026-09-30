.syntax unified
	.thumb
	.global Func_081c0490
	.thumb_func
Func_081c0490:
	adds r2, r0, #0
	ldr r3, [r2, #52]
	ldr r0, .L_081c04a4
	cmp r3, r0
	bne .L_081c04a2
	ldr r0, [r2, #4]
	ldr r1, .L_081c04a8
	ands r0, r1
	str r0, [r2, #4]
.L_081c04a2:
	bx lr
.L_081c04a4:
	.4byte 0x68736d53
.L_081c04a8:
	.4byte 0x7fffffff
