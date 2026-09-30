.syntax unified
	.thumb
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	movs	r3, #7
	ands	r3, r0
	ldr	r1, [pc, #16]
	movs	r2, #1
	lsls	r2, r3
	lsls	r3, r0, #20
	lsrs	r0, r3, #23
	ldrb	r3, [r1, r0]
	orrs	r2, r3
	strb	r2, [r1, r0]
	bx	lr
	movs	r0, r0
	.2byte 0x0040
	.2byte 0x0200
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	movs	r3, #7
	ands	r3, r0
	ldr	r1, [pc, #16]
	movs	r2, #1
	lsls	r2, r3
	lsls	r3, r0, #20
	lsrs	r0, r3, #23
	ldrb	r3, [r1, r0]
	bics	r3, r2
	strb	r3, [r1, r0]
	bx	lr
	movs	r0, r0
	.2byte 0x0040
	.2byte 0x0200
	.global Func_08016d34
	.thumb_func
Func_08016d34:
	adds	r4, r0, #0
	movs	r3, #7
	ldr	r2, [pc, #28]
	ands	r3, r4
	movs	r1, #1
	lsls	r1, r3
	lsls	r3, r4, #20
	lsrs	r4, r3, #23
	ldrb	r0, [r2, r4]
	adds	r3, r1, #0
	eors	r3, r0
	strb	r3, [r2, r4]
	ldrb	r3, [r2, r4]
	ands	r3, r1
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	bx	lr
	.4byte 0x02000040
