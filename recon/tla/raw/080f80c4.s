.syntax unified
	.thumb
	push	{lr}
	movs	r0, #169
	lsls	r0, r0, #1
	bl	GameFlag_SetBitFar
	movs	r0, #179
	lsls	r0, r0, #1
	bl	GameFlag_SetBitFar
	bl	0x080202d8
	bl	0x080146d4
	pop	{pc}
	movs	r3, #192
	lsls	r3, r3, #18
	sub	sp, #4
	adds	r3, #220
	ldr	r4, [r3, #0]
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r1, r4, #0
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #255
	strb	r3, [r4, #28]
	adds	r2, r4, #0
	movs	r3, #1
	strb	r3, [r4, #30]
	strb	r3, [r4, #31]
	adds	r2, #246
	adds	r4, #247
	strb	r3, [r2, #0]
	add	sp, #4
	strb	r3, [r4, #0]
	bx	lr
	movs	r0, r0
	.2byte 0x033b
	.2byte 0x8500
