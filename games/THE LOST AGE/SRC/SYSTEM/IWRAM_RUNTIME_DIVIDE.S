@ Division and remainder of TLA's resident runtime, taking the dividend in
@ r0 and the divisor in r1. Divisors below 0x4000 use the reciprocal table
@ at 0x080028a8 (umulls; mulne; subne; bxne lr), larger ones an unrolled
@ restoring loop (rsbs; orrcs; subcs). The signed divide branches on the
@ carry out of lsrs r0, ip, #32, and its Thumb entries run into the shared
@ core as the ARM instructions andeq r4, r0, #0x1e00000. GCC never
@ branches on the carry out of a shift or runs one entry into another.
@ Its five 4-byte Thumb entries, which are TLA's own, carry no credit.
@ credit: handwritten — TLA IWRAM runtime division helpers
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08000a60
	.set AlchemyRunAddress, 0x030004a8
.L_030004a8:
	eor	ip, r0, r1
	movs	r2, r1
	rsbmi	r2, r2, #0
	movs	r1, r0
	rsbmi	r1, r1, #0
	lsrs	r0, ip, #32
	bcc	.L_03000544
	mov	ip, lr
	bl	.L_03000544
	rsb	r0, r0, #0
	bx	ip
.L_030004d4:
	stmfd	sp!, {lr}
	eor	ip, r0, r1
	movs	r2, r1
	rsbmi	r2, r2, #0
	movs	r1, r0
	rsbmi	r1, r1, #0
	mov	r0, #0
	bl	.L_03000544
	mov	r0, r1
	movs	ip, ip
	rsbmi	r0, r0, #0
	ldmfd	sp!, {lr}
	bx	lr
	.thumb
AlchemyUncredited_08000ac0:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08000ac0:
	orrs	r3, r0, r1
	bmi	.L_030004d4
AlchemyUncredited_08000acc:
	andeq	r4, r0, #120, 14
AlchemyUncreditedEnd_08000acc:
	mov	ip, lr
	bl	.L_03000534
	mov	r0, r1
	bx	ip
	.thumb
AlchemyUncredited_08000ae0:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08000ae0:
	orrs	r3, r0, r1
	bmi	.L_030004a8
.L_03000534:
AlchemyUncredited_08000aec:
	andeq	r4, r0, #120, 14
AlchemyUncreditedEnd_08000aec:
	mov	r2, r1
	mov	r1, r0
	mov	r0, #0
.L_03000544:
AlchemyUncredited_08000afc:
	andeq	r4, r0, #120, 14
AlchemyUncreditedEnd_08000afc:
	cmp	r2, #0x4000
	bcs	.L_03000578
	ldr	r3, [pc, #28]
	ldr	r3, [r3, r2, lsl #2]
	umulls	r3, r0, r1, r3
	mulne	r3, r0, r2
	subne	r1, r1, r3
	bxne	lr
	mov	r0, r1
	mov	r1, #0
	bx	lr
	.4byte	0x080028a8
.L_03000578:
	rsbs	r3, r2, r1, lsr #16
	bcc	.L_030005b0
	rsbs	r3, r2, r1, lsr #19
	orrcs	r0, r0, #0x80000
	subcs	r1, r1, r2, lsl #19
	rsbs	r3, r2, r1, lsr #18
	orrcs	r0, r0, #0x40000
	subcs	r1, r1, r2, lsl #18
	rsbs	r3, r2, r1, lsr #17
	orrcs	r0, r0, #0x20000
	subcs	r1, r1, r2, lsl #17
	rsbs	r3, r2, r1, lsr #16
	orrcs	r0, r0, #0x10000
	subcs	r1, r1, r2, lsl #16
.L_030005b0:
	rsbs	r3, r2, r1, lsr #12
	bcc	.L_030005e8
	rsbs	r3, r2, r1, lsr #15
	orrcs	r0, r0, #0x8000
	subcs	r1, r1, r2, lsl #15
	rsbs	r3, r2, r1, lsr #14
	orrcs	r0, r0, #0x4000
	subcs	r1, r1, r2, lsl #14
	rsbs	r3, r2, r1, lsr #13
	orrcs	r0, r0, #0x2000
	subcs	r1, r1, r2, lsl #13
	rsbs	r3, r2, r1, lsr #12
	orrcs	r0, r0, #0x1000
	subcs	r1, r1, r2, lsl #12
.L_030005e8:
	rsbs	r3, r2, r1, lsr #8
	bcc	.L_03000620
	rsbs	r3, r2, r1, lsr #11
	orrcs	r0, r0, #0x800
	subcs	r1, r1, r2, lsl #11
	rsbs	r3, r2, r1, lsr #10
	orrcs	r0, r0, #0x400
	subcs	r1, r1, r2, lsl #10
	rsbs	r3, r2, r1, lsr #9
	orrcs	r0, r0, #0x200
	subcs	r1, r1, r2, lsl #9
	rsbs	r3, r2, r1, lsr #8
	orrcs	r0, r0, #0x100
	subcs	r1, r1, r2, lsl #8
.L_03000620:
	rsbs	r3, r2, r1, lsr #4
	bcc	.L_03000658
	rsbs	r3, r2, r1, lsr #7
	orrcs	r0, r0, #128
	subcs	r1, r1, r2, lsl #7
	rsbs	r3, r2, r1, lsr #6
	orrcs	r0, r0, #64
	subcs	r1, r1, r2, lsl #6
	rsbs	r3, r2, r1, lsr #5
	orrcs	r0, r0, #32
	subcs	r1, r1, r2, lsl #5
	rsbs	r3, r2, r1, lsr #4
	orrcs	r0, r0, #16
	subcs	r1, r1, r2, lsl #4
.L_03000658:
	rsbs	r3, r2, r1
	bxcc	lr
	rsbs	r3, r2, r1, lsr #3
	orrcs	r0, r0, #8
	subcs	r1, r1, r2, lsl #3
	rsbs	r3, r2, r1, lsr #2
	orrcs	r0, r0, #4
	subcs	r1, r1, r2, lsl #2
	rsbs	r3, r2, r1, lsr #1
	orrcs	r0, r0, #2
	subcs	r1, r1, r2, lsl #1
	rsbs	r3, r2, r1
	orrcs	r0, r0, #1
	subcs	r1, r1, r2
	bx	lr
