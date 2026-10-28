	.include "games/tla/include/ed_asm.h"
.syntax unified
	.thumb
	.global IwramClearWords
IwramClearWords:
	bx	pc
AlchemyUncredited_08000812:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08000812:
	mov	r2, #0
	.global IwramFillWords
IwramFillWords:
	andeq	r4, r0, #120, 14
	push	{r5, r6, r7, r8, r9}
	mov	r3, r2
	mov	r4, r2
	mov	r5, r2
	mov	r6, r2
	mov	r7, r2
	mov	r8, r2
	mov	r9, r2
	ands	ip, r1, #224
	rsb	ip, ip, #224
	add	pc, pc, ip, lsr #3
.L_03000290:
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	subs	r1, r1, #0x100
	bpl	.L_03000290
	ands	r1, r1, #28
	beq	.L_030002cc
.L_030002c0:
	stmia	r0!, {r2}
	subs	r1, r1, #4
	bgt	.L_030002c0
.L_030002cc:
	pop	{r5, r6, r7, r8, r9}
	bx	lr
	.thumb
	.global IwramSqrt
IwramSqrt:
	bx	pc
AlchemyUncredited_0800088e:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_0800088e:
	movs	r1, r0
	mov	r0, #0
	bxmi	lr
	mov	r2, #0x8000
.L_030002e8:
	add	r0, r0, r2
	mul	r3, r0, r0
	cmp	r3, r1
	subhi	r0, r0, r2
	add	r0, r0, r2, lsr #1
	mul	r3, r0, r0
	cmp	r3, r1
	subhi	r0, r0, r2, lsr #1
	lsrs	r2, r2, #2
	bne	.L_030002e8
	bx	lr
	.thumb
	bx	pc
AlchemyUncredited_080008ce:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080008ce:
	.ifeq EDITION_INTERNATIONAL
	add	ip, r0, #24
	.else
	add	ip, r0, #28
	.endif
.L_0300031c:
	mov	r4, r1
	ldrh	r3, [r0], #2
	bic	r3, r3, #3
	lsls	r3, r3, #17
	bcc	.L_0300033c
.L_03000330:
	strb	r2, [r4], #1
.L_03000334:
	lsls	r3, r3, #1
	bcs	.L_03000330
.L_0300033c:
	add	r4, r4, #1
	bne	.L_03000334
	add	r1, r1, #16
	cmp	r0, ip
	bne	.L_0300031c
	bx	lr
