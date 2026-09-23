.syntax unified
	.thumb
	.set sub_08014128, 0x08014128
	.global Func_0803f698
	.thumb_func
Func_0803f698:
	push	{lr}
	ldr	r3, [pc, #24]
	ldrh	r4, [r0, #6]
	ands	r1, r3
	ldr	r3, [pc, #20]
	strb	r2, [r0, #4]
	ands	r3, r4
	orrs	r3, r1
	strh	r3, [r0, #6]
	movs	r1, #252
	bl	sub_08014128
	b.n	.L_0803f6bc
	movs	r0, r0
	.4byte 0x000001ff
	.2byte 0xfe00
	.2byte 0xffff
.L_0803f6bc:
	pop	{pc}
	.2byte 0x0000
