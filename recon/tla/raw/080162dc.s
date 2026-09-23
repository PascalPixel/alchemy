.syntax unified
	.thumb
	.global Func_080162dc
	.thumb_func
Func_080162dc:
	push	{lr}
	ldr	r2, [pc, #12]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080162ea
	movs	r3, #1
	strb	r3, [r2, #8]
.L_080162ea:
	pop	{pc}
	.2byte 0x5360
	.2byte 0x0200
	ldr	r0, [pc, #64]
	ldr	r4, [pc, #48]
	ldr	r1, [pc, #64]
	strh	r4, [r0, #0]
	movs	r3, #255
	ldrh	r2, [r1, #0]
	lsls	r3, r3, #8
	adds	r3, #63
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r3, [pc, #32]
	ldr	r2, [pc, #52]
	strh	r3, [r0, #0]
	ldr	r3, [pc, #32]
	strh	r3, [r2, #0]
	movs	r3, #201
	lsls	r3, r3, #8
	subs	r2, #28
	adds	r3, #99
	str	r3, [r2, #0]
	ldr	r3, [pc, #20]
	adds	r2, #246
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	strb	r4, [r3, #8]
	b.n	.L_08016344
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00002003
	.4byte 0x000000c0
	.4byte 0x04000208
	.4byte 0x04000200
	.4byte 0x04000128
	.2byte 0x5360
	.2byte 0x0200
.L_08016344:
	bx	lr
	.2byte 0x0000
