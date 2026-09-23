@ Twelve battle-effect ARM kernels that the bank copies out and runs; the
@ tables at 0x08196d74 and 0x081969a4 give their sizes and addresses. Each
@ sits behind a 4-byte Thumb entry (bx pc). This module holds kernels 1 to
@ 3; RELOCATED_KERNEL_4.S to RELOCATED_KERNEL_11.S hold the rest. Each of
@ these three clears bit 0 of its closing literal 0x080028a9 (0x081391bc,
@ 0x081393cc and 0x081395ec), the address of the reciprocal table at
@ 0x080028a8, at run time (ldr fp, [pc, #484]; bic fp, fp, #1;
@ str fp, [pc, #476]; 500 and 492 in kernel 2, 516 and 508 in kernel 3),
@ loads it again as that table's address and multiplies with the
@ flag-setting umulls; they store to no other PC-relative address. GCC 2.96
@ never stores to a PC-relative address and has no flag-setting long
@ multiply. The entries, which are TLA's own, and the three literals, words
@ rewritten at run time, carry no credit. TBS has no counterpart; no TBS
@ battle-effect module shares a 64-byte run with them.
@ credit: handwritten — relocated ARM kernels 1 to 3 of the battle-effect bank
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08138fc8
	.global Func_08138fc8
	.thumb_func
Func_08138fc8:
AlchemyUncredited_08138fc8:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08138fc8:
	push	{r5, r6, r7, r8, r9, sl, fp, ip, lr}
	ldr	fp, [pc, #484]
	bic	fp, fp, #1
	str	fp, [pc, #476]
	ldr	r1, [r0, #12]
	ldr	fp, [r0, #20]
	ldr	r9, [r0, #4]
	ldr	r0, [r0, #8]
	mov	lr, #0x3000000
	ldr	lr, [lr, #96]
.L_08138ff4:
	ldr	r3, [r0], #4
	lsls	r5, r3, #8
	beq	.L_081391c0
	add	fp, fp, #1
	and	r5, r3, #255
	ldr	r6, [r1, r5, lsl #3]
	and	r5, r3, #0xff00
	ldr	r7, [r1, r5, lsr #5]
	and	r5, r3, #0xff0000
	ldr	r8, [r1, r5, lsr #13]
	tst	r9, #3
	beq	.L_08139060
	asr	r5, r8, #16
	sub	r5, r5, r6, asr #16
	sub	r4, r7, r6
	lsl	r4, r4, #16
	asr	r4, r4, #16
	mul	r3, r4, r5
	asr	r5, r6, #16
	sub	r5, r5, r7, asr #16
	sub	r4, r8, r6
	lsl	r4, r4, #16
	asr	r4, r4, #16
	mlas	r3, r4, r5, r3
	beq	.L_08138ff4
	eors	r3, r3, r9, lsl #31
	bpl	.L_08138ff4
.L_08139060:
	cmp	r6, r7
	eorgt	r6, r6, r7
	eorgt	r7, r7, r6
	eorgt	r6, r6, r7
	cmp	r7, r8
	ble	.L_08139094
	eor	r7, r7, r8
	eor	r8, r8, r7
	eor	r7, r7, r8
	cmp	r6, r7
	eorgt	r6, r6, r7
	eorgt	r7, r7, r6
	eorgt	r6, r6, r7
.L_08139094:
	asrs	r3, r6, #23
	bgt	.L_08138ff4
	asrs	r3, r6, #16
	rsbs	r4, r3, r8, asr #16
	beq	.L_08138ff4
	push	{r0, r1, r9}
	lsls	r6, r6, #16
	rsbs	r9, r6, r8, lsl #16
	rsbmi	r9, r9, #0
	ldr	r5, [pc, #252]
	ldr	sl, [r5, r4, lsl #2]
	umulls	r5, ip, sl, r9
	moveq	ip, r9
	rsbs	r9, r6, r8, lsl #16
	rsbmi	ip, ip, #0
	mov	r5, r6
	ldrb	r1, [fp, #-1]
.L_081390d8:
	rsbs	r0, r3, r7, asr #16
	beq	.L_081391a4
	rsbs	r9, r6, r7, lsl #16
	rsbmi	r9, r9, #0
	ldr	r4, [pc, #204]
	ldr	sl, [r4, r0, lsl #2]
	umulls	r4, r0, sl, r9
	moveq	r0, r9
	rsbs	r9, r6, r7, lsl #16
	rsbmi	r0, r0, #0
	cmp	r7, #0x800000
	movgt	r7, #0x800000
	bgt	.L_0813919c
.L_0813910c:
	asrs	r4, r3, #3
	bmi	.L_08139190
	asrs	r9, r5, #16
	subs	sl, r9, r6, asr #16
	rsbmi	sl, sl, #0
	asrgt	r9, r6, #16
	asrs	r4, r9, #7
	bgt	.L_08139190
	addmi	sl, sl, r9
	movmi	r9, #0
	adds	r4, r9, sl
	ble	.L_08139190
	subs	r4, r4, #128
	subsgt	sl, sl, r4
	cmp	sl, #0
	ble	.L_08139190
	asrs	r4, r3, #3
	and	r2, r3, #7
	lsls	r2, r2, #3
	orr	r2, r2, r4, lsl #10
	add	r2, r2, lr
	bic	r4, r9, #7
	orr	r9, r4, r9, lsl #29
	tst	sl, #1
	bne	.L_0813917c
.L_08139170:
	strb	r1, [r2, r9, ror #29]
	adds	r9, r9, #0x20000000
	addcs	r9, r9, #8
.L_0813917c:
	strb	r1, [r2, r9, ror #29]
	adds	r9, r9, #0x20000000
	addcs	r9, r9, #8
	subs	sl, sl, #2
	bgt	.L_08139170
.L_08139190:
	add	r5, r5, ip
	add	r6, r6, r0
	add	r3, r3, #1
.L_0813919c:
	subs	r4, r3, r7, asr #16
	bne	.L_0813910c
.L_081391a4:
	lsl	r6, r7, #16
	movs	r7, r8
	mov	r8, #0
	bne	.L_081390d8
	pop	{r0, r1, r9}
	b	.L_08138ff4
AlchemyUncredited_081391bc:
	.4byte	0x080028a9
.L_081391c0:
AlchemyUncreditedEnd_081391bc:
	pop	{r5, r6, r7, r8, r9, sl, fp, ip, lr}
	bx	lr
	.thumb
	.global Func_081391c8
	.thumb_func
Func_081391c8:
AlchemyUncredited_081391c8:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_081391c8:
	push	{r5, r6, r7, r8, r9, sl, fp, ip, lr}
	ldr	fp, [pc, #500]
	bic	fp, fp, #1
	str	fp, [pc, #492]
	ldr	r1, [r0, #12]
	ldr	fp, [r0, #20]
	ldr	r9, [r0, #4]
	ldr	r0, [r0, #8]
	mov	lr, #0x3000000
	ldr	lr, [lr, #96]
.L_081391f4:
	ldr	r3, [r0], #4
	lsls	r5, r3, #8
	beq	.L_081393d0
	add	fp, fp, #1
	and	r5, r3, #255
	ldr	r6, [r1, r5, lsl #3]
	and	r5, r3, #0xff00
	ldr	r7, [r1, r5, lsr #5]
	and	r5, r3, #0xff0000
	ldr	r8, [r1, r5, lsr #13]
	tst	r9, #3
	beq	.L_08139260
	asr	r5, r8, #16
	sub	r5, r5, r6, asr #16
	sub	r4, r7, r6
	lsl	r4, r4, #16
	asr	r4, r4, #16
	mul	r3, r4, r5
	asr	r5, r6, #16
	sub	r5, r5, r7, asr #16
	sub	r4, r8, r6
	lsl	r4, r4, #16
	asr	r4, r4, #16
	mlas	r3, r4, r5, r3
	beq	.L_081391f4
	eors	r3, r3, r9, lsl #31
	bpl	.L_081391f4
.L_08139260:
	cmp	r6, r7
	eorgt	r6, r6, r7
	eorgt	r7, r7, r6
	eorgt	r6, r6, r7
	cmp	r7, r8
	ble	.L_08139294
	eor	r7, r7, r8
	eor	r8, r8, r7
	eor	r7, r7, r8
	cmp	r6, r7
	eorgt	r6, r6, r7
	eorgt	r7, r7, r6
	eorgt	r6, r6, r7
.L_08139294:
	asrs	r3, r6, #23
	bgt	.L_081391f4
	asrs	r3, r6, #16
	rsbs	r4, r3, r8, asr #16
	beq	.L_081391f4
	push	{r0, r1, r9}
	lsls	r6, r6, #16
	rsbs	r9, r6, r8, lsl #16
	rsbmi	r9, r9, #0
	ldr	r5, [pc, #268]
	ldr	sl, [r5, r4, lsl #2]
	umulls	r5, ip, sl, r9
	moveq	ip, r9
	rsbs	r9, r6, r8, lsl #16
	rsbmi	ip, ip, #0
	mov	r5, r6
	ldrb	r1, [fp, #-1]
.L_081392d8:
	rsbs	r0, r3, r7, asr #16
	beq	.L_081393b4
	rsbs	r9, r6, r7, lsl #16
	rsbmi	r9, r9, #0
	ldr	r4, [pc, #220]
	ldr	sl, [r4, r0, lsl #2]
	umulls	r4, r0, sl, r9
	moveq	r0, r9
	rsbs	r9, r6, r7, lsl #16
	rsbmi	r0, r0, #0
	cmp	r7, #0x800000
	movgt	r7, #0x800000
	bgt	.L_081393ac
.L_0813930c:
	asrs	r4, r3, #3
	bmi	.L_081393a0
	asrs	r9, r5, #16
	subs	sl, r9, r6, asr #16
	rsbmi	sl, sl, #0
	asrgt	r9, r6, #16
	asrs	r4, r9, #7
	bgt	.L_081393a0
	addmi	sl, sl, r9
	movmi	r9, #0
	adds	r4, r9, sl
	ble	.L_081393a0
	subs	r4, r4, #128
	subsgt	sl, sl, r4
	cmp	sl, #0
	ble	.L_081393a0
	asrs	r4, r3, #3
	and	r2, r3, #7
	lsls	r2, r2, #3
	orr	r2, r2, r4, lsl #10
	add	r2, r2, lr
	bic	r4, r9, #7
	orr	r9, r4, r9, lsl #29
	tst	sl, #1
	bne	.L_08139384
.L_08139370:
	ldrb	r4, [r2, r9, ror #29]
	cmp	r4, r1
	strbcc	r1, [r2, r9, ror #29]
	adds	r9, r9, #0x20000000
	addcs	r9, r9, #8
.L_08139384:
	ldrb	r4, [r2, r9, ror #29]
	cmp	r4, r1
	strbcc	r1, [r2, r9, ror #29]
	adds	r9, r9, #0x20000000
	addcs	r9, r9, #8
	subs	sl, sl, #2
	bgt	.L_08139370
.L_081393a0:
	add	r5, r5, ip
	add	r6, r6, r0
	add	r3, r3, #1
.L_081393ac:
	subs	r4, r3, r7, asr #16
	bne	.L_0813930c
.L_081393b4:
	lsl	r6, r7, #16
	movs	r7, r8
	mov	r8, #0
	bne	.L_081392d8
	pop	{r0, r1, r9}
	b	.L_081391f4
AlchemyUncredited_081393cc:
	.4byte	0x080028a9
.L_081393d0:
AlchemyUncreditedEnd_081393cc:
	pop	{r5, r6, r7, r8, r9, sl, fp, ip, lr}
	bx	lr
	.thumb
	.global Func_081393d8
	.thumb_func
Func_081393d8:
AlchemyUncredited_081393d8:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_081393d8:
	push	{r5, r6, r7, r8, r9, sl, fp, ip, lr}
	ldr	fp, [pc, #516]
	bic	fp, fp, #1
	str	fp, [pc, #508]
	ldr	r1, [r0, #12]
	ldr	fp, [r0, #20]
	ldr	r9, [r0, #4]
	ldr	r0, [r0, #8]
	mov	lr, #0x3000000
	ldr	lr, [lr, #96]
.L_08139404:
	ldr	r3, [r0], #4
	lsls	r5, r3, #8
	beq	.L_081395f0
	add	fp, fp, #1
	and	r5, r3, #255
	ldr	r6, [r1, r5, lsl #3]
	and	r5, r3, #0xff00
	ldr	r7, [r1, r5, lsr #5]
	and	r5, r3, #0xff0000
	ldr	r8, [r1, r5, lsr #13]
	tst	r9, #3
	beq	.L_08139470
	asr	r5, r8, #16
	sub	r5, r5, r6, asr #16
	sub	r4, r7, r6
	lsl	r4, r4, #16
	asr	r4, r4, #16
	mul	r3, r4, r5
	asr	r5, r6, #16
	sub	r5, r5, r7, asr #16
	sub	r4, r8, r6
	lsl	r4, r4, #16
	asr	r4, r4, #16
	mlas	r3, r4, r5, r3
	beq	.L_08139404
	eors	r3, r3, r9, lsl #31
	bpl	.L_08139404
.L_08139470:
	cmp	r6, r7
	eorgt	r6, r6, r7
	eorgt	r7, r7, r6
	eorgt	r6, r6, r7
	cmp	r7, r8
	ble	.L_081394a4
	eor	r7, r7, r8
	eor	r8, r8, r7
	eor	r7, r7, r8
	cmp	r6, r7
	eorgt	r6, r6, r7
	eorgt	r7, r7, r6
	eorgt	r6, r6, r7
.L_081394a4:
	asrs	r3, r6, #23
	bgt	.L_08139404
	asrs	r3, r6, #16
	rsbs	r4, r3, r8, asr #16
	beq	.L_08139404
	push	{r0, r1, r9}
	lsls	r6, r6, #16
	rsbs	r9, r6, r8, lsl #16
	rsbmi	r9, r9, #0
	ldr	r5, [pc, #284]
	ldr	sl, [r5, r4, lsl #2]
	umulls	r5, ip, sl, r9
	moveq	ip, r9
	rsbs	r9, r6, r8, lsl #16
	rsbmi	ip, ip, #0
	mov	r5, r6
	ldrb	r1, [fp, #-1]
.L_081394e8:
	rsbs	r0, r3, r7, asr #16
	beq	.L_081395d4
	rsbs	r9, r6, r7, lsl #16
	rsbmi	r9, r9, #0
	ldr	r4, [pc, #236]
	ldr	sl, [r4, r0, lsl #2]
	umulls	r4, r0, sl, r9
	moveq	r0, r9
	rsbs	r9, r6, r7, lsl #16
	rsbmi	r0, r0, #0
	cmp	r7, #0x800000
	movgt	r7, #0x800000
	bgt	.L_081395cc
.L_0813951c:
	asrs	r4, r3, #3
	bmi	.L_081395c0
	asrs	r9, r5, #16
	subs	sl, r9, r6, asr #16
	rsbmi	sl, sl, #0
	asrgt	r9, r6, #16
	asrs	r4, r9, #7
	bgt	.L_081395c0
	addmi	sl, sl, r9
	movmi	r9, #0
	adds	r4, r9, sl
	ble	.L_081395c0
	subs	r4, r4, #128
	subsgt	sl, sl, r4
	cmp	sl, #0
	ble	.L_081395c0
	asrs	r4, r3, #3
	and	r2, r3, #7
	lsls	r2, r2, #3
	orr	r2, r2, r4, lsl #10
	add	r2, r2, lr
	bic	r4, r9, #7
	orr	r9, r4, r9, lsl #29
	tst	sl, #1
	bne	.L_0813959c
.L_08139580:
	ldrb	r4, [r2, r9, ror #29]
	add	r4, r4, r1
	cmp	r4, #63
	movge	r4, #63
	strb	r4, [r2, r9, ror #29]
	adds	r9, r9, #0x20000000
	addcs	r9, r9, #8
.L_0813959c:
	ldrb	r4, [r2, r9, ror #29]
	add	r4, r4, r1
	cmp	r4, #63
	movge	r4, #63
	strb	r4, [r2, r9, ror #29]
	adds	r9, r9, #0x20000000
	addcs	r9, r9, #8
	subs	sl, sl, #2
	bgt	.L_08139580
.L_081395c0:
	add	r5, r5, ip
	add	r6, r6, r0
	add	r3, r3, #1
.L_081395cc:
	subs	r4, r3, r7, asr #16
	bne	.L_0813951c
.L_081395d4:
	lsl	r6, r7, #16
	movs	r7, r8
	mov	r8, #0
	bne	.L_081394e8
	pop	{r0, r1, r9}
	b	.L_08139404
AlchemyUncredited_081395ec:
	.4byte	0x080028a9
.L_081395f0:
AlchemyUncreditedEnd_081395ec:
	pop	{r5, r6, r7, r8, r9, sl, fp, ip, lr}
	bx	lr
