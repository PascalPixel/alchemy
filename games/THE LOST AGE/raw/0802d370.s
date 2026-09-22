.syntax unified
	.thumb
	.global Overlay_0802d370
Overlay_0802d370:
	adds	r3, r0, #0
	movs	r0, #0
	ldrsb	r0, [r3, r0]
	movs	r4, #1
	ldrsb	r4, [r3, r4]
	ldr	r3, [pc, #20]
	lsls	r2, r2, #4
	adds	r1, r1, r2
	ldrb	r3, [r3, r1]
	lsls	r0, r0, #19
	lsls	r4, r4, #19
	subs	r4, r4, r0
	muls	r3, r4
	adds	r0, r0, r3
	bx	lr
	movs	r0, r0
	.2byte 0xedc4
	.2byte 0x0802
	push	{lr}
	cmp	r1, #7
	bhi.n	.L_0802d3a2
	ldrb	r0, [r0, #0]
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	b.n	.L_0802d3a8
.L_0802d3a2:
	ldrb	r0, [r0, #1]
	lsls	r0, r0, #24
	asrs	r0, r0, #24
.L_0802d3a8:
	lsls	r0, r0, #19
	pop	{pc}
	push	{lr}
	cmp	r2, #7
	bhi.n	.L_0802d3ba
	ldrb	r0, [r0, #0]
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	b.n	.L_0802d3c0
.L_0802d3ba:
	ldrb	r0, [r0, #1]
	lsls	r0, r0, #24
	asrs	r0, r0, #24
.L_0802d3c0:
	lsls	r0, r0, #19
	pop	{pc}
	ldr	r3, [pc, #12]
	lsls	r1, r1, #4
	adds	r2, r2, r1
	ldrb	r3, [r3, r2]
	ldrsb	r0, [r0, r3]
	lsls	r0, r0, #19
	bx	lr
	movs	r0, r0
	.2byte 0xeec4
	.2byte 0x0802
	ldr	r3, [pc, #12]
	lsls	r1, r1, #4
	subs	r1, r1, r2
	adds	r1, #15
	ldrb	r3, [r3, r1]
	ldrsb	r0, [r0, r3]
	lsls	r0, r0, #19
	bx	lr
	.4byte 0x0802eec4
