@ Battle-effect kernel 10, one of the bank described in RELOCATED_KERNELS.S,
@ from its 4-byte Thumb entry (bx pc) at 0x0813b2ec to its return (pop
@ {r5-fp, lr}; bx lr) at 0x0813b798. The entry branches to set-up code at
@ 0x0813b6c4, which clears bit 0 of the kernel's closing literal 0x080028a9
@ at 0x0813b784, the address of the reciprocal table at 0x080028a8 (ldr fp,
@ [pc, #180]; bic fp, fp, #1; str fp, [pc, #172]), ORs shift amounts into
@ five of its own instructions (ldr r3, [pc, #-N]; orr r3, r3, r5, lsl #7;
@ str r3, [pc, #-N]), stores the four words after that literal and has DMA3
@ copy the seven instruction words at 0x0813b610 over the seven zero words
@ that follow them (0x0813b62c; control 0x84000007) before it enters its
@ loop, which then runs both copies and multiplies with umulls and umullsne.
@ The loop also saves two words just after the entry (0x0813b2f4; sub r0,
@ pc, #236; stm r0, {r7, r8}). Rewriting its own instructions, storing to
@ PC-relative addresses and flag-setting long multiplies are beyond both
@ approved compilers. The entry, which is TLA's own, the two and four
@ variable words, the zero words, which never run as stored, and the
@ literal, a word rewritten at run time, carry no credit.
@ credit: handwritten — relocated self-modifying ARM kernel 10 of the battle-effect bank
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x0813b2ec
	.global Func_0813b2ec
	.thumb_func
Func_0813b2ec:
AlchemyUncredited_0813b2ec:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_0813b2ec:
	b	.L_0813b6c4
AlchemyUncredited_0813b2f4:
	.4byte	0
	.4byte	0
AlchemyUncreditedEnd_0813b2f4:
.L_0813b2fc:
	add	sp, sp, #64
	pop	{r0, r1, fp}
.L_0813b304:
	ldm	r0!, {r3, r9, sl}
	lsls	r5, r3, #8
	beq	.L_0813b798
	and	r5, r3, #255
	ldr	r6, [r1, r5, lsl #3]
	and	r5, r3, #0xff00
	ldr	r7, [r1, r5, lsr #5]
	and	r5, r3, #0xff0000
	ldr	r8, [r1, r5, lsr #13]
	tst	fp, #3
	beq	.L_0813b36c
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
	beq	.L_0813b304
	eors	r3, r3, fp, lsl #31
	bpl	.L_0813b304
.L_0813b36c:
	cmp	r6, r7
	eorgt	r6, r6, r7
	eorgt	r7, r7, r6
	eorgt	r6, r6, r7
	rorgt	r9, r9, #16
	tst	r7, r8
	bmi	.L_0813b304
	cmp	r7, r8
	ble	.L_0813b3bc
	eor	r7, r7, r8
	eor	r8, r8, r7
	eor	r7, r7, r8
	lsl	r5, sl, #16
	ror	sl, r9, #16
	orr	r9, r5, sl, lsr #16
	cmp	r6, r7
	eorgt	r6, r6, r7
	eorgt	r7, r7, r6
	eorgt	r6, r6, r7
	rorgt	r9, r9, #16
.L_0813b3bc:
	asrs	r3, r6, #22
	bgt	.L_0813b304
	asrs	r3, r6, #16
	rsbs	r4, r3, r8, asr #16
	beq	.L_0813b304
	push	{r0, r1, fp}
	sub	sp, sp, #64
	sub	r0, pc, #236
	stm	r0, {r7, r8}
	lsls	r6, r6, #16
	rsbs	r0, r6, r8, lsl #16
	rsbmi	r0, r0, #0
	ldr	r5, [pc, #912]
	ldr	r1, [r5, r4, lsl #2]
	umulls	r5, ip, r1, r0
	moveq	ip, r0
	rsbs	r0, r6, r8, lsl #16
	rsbmi	ip, ip, #0
	str	ip, [sp, #8]
	and	r0, r9, #0xff00
	sub	r0, sl, r0
	lsls	r2, r0, #16
	rsbmi	r2, r2, #0
	umulls	r5, r4, r1, r2
	moveq	r4, r2
	lsls	r0, r0, #16
	rsbmi	r4, r4, #0
	sub	r0, sl, r9
	lsls	r2, r0, #24
	rsbmi	r2, r2, #0
	cmp	r1, #0
	umullsne	r5, r2, r1, r2
	lsls	r0, r0, #24
	rsbmi	r2, r2, #0
	ldr	r0, [pc, #840]
	add	sl, r9, r0
	add	sl, sl, sl
	add	sl, sl, #1
	.inst	0xe1a0a00a
	str	sl, [sp]
	.inst	0xe1a0a002
	str	sl, [sp, #16]
	lsr	r0, r0, #8
	lsl	fp, r9, #16
	and	fp, fp, #0xff000000
	orr	sl, fp, #0x800000
	add	sl, sl, r0, lsl #24
	str	sl, [sp, #4]
	lsl	sl, r9, #24
	str	r4, [sp, #20]
	str	r7, [sp, #24]
	rsbs	r0, r3, r7, asr #16
	mlane	fp, r4, r0, fp
	mlane	sl, r2, r0, sl
	mla	r1, ip, r0, r6
	rsb	r1, r1, r7, lsl #16
	asrs	r1, r1, #16
	beq	.L_0813b2fc
	ldr	r5, [pc, #728]
	ldr	r2, [r5, r0, lsl #2]
	rsbs	r0, r6, r7, lsl #16
	rsbmi	r0, r0, #0
	cmp	r2, #0
	umullsne	lr, r0, r2, r0
	rsbs	r2, r6, r7, lsl #16
	rsbmi	r0, r0, #0
	str	r0, [sp, #12]
	movs	r2, r1
	rsbmi	r2, r2, #0
	ldr	r2, [r5, r2, lsl #2]
	and	r4, r9, #0xff000000
	subs	r4, r4, fp
	eor	r5, r1, r4
	rsbmi	r4, r4, #0
	cmp	r2, #0
	umullsne	lr, r4, r2, r4
	movs	r5, r5
	rsbmi	r4, r4, #0
	and	r9, r9, #0xff0000
	rsbs	r9, sl, r9, lsl #8
	eor	r5, r1, r9
	rsbmi	r9, r9, #0
	cmp	r2, #0
	umullsne	lr, r9, r2, r9
	movs	r5, r5
	rsbmi	r9, r9, #0
	lsr	r9, r9, #16
	.inst	0xe1a09809
	orr	r9, r9, r4, lsr #16
	asrs	r1, r1, #31
	submi	r6, r6, #0x10000
	mov	r5, r6
	rsbmi	r9, r9, #0
	movpl	r1, #1
	lsls	r1, r1, #29
	orrpl	r1, r1, #8
.L_0813b53c:
	ldr	r4, [sp, #24]
	asr	r4, r4, #16
	cmp	r4, #64
	movge	r4, #64
	str	r4, [sp, #24]
	cmp	r3, r4
	beq	.L_0813b674
	ldm	sp, {r7, r8}
.L_0813b55c:
	asrs	sl, r3, #3
	bmi	.L_0813b650
	asrs	r0, r6, #22
	mvnmi	ip, #0
	movpl	ip, #64
	asreq	ip, r6, #16
	asrs	r4, r5, #16
	asrs	fp, r4, #6
	beq	.L_0813b5b8
	bmi	.L_0813b598
	cmp	r6, #0x400000
	bge	.L_0813b650
	sub	fp, r4, #63
	mov	r4, #63
	b	.L_0813b5a8
.L_0813b598:
	movs	r6, r6
	bmi	.L_0813b650
	rsb	fp, r4, #0
	mov	r4, #0
.L_0813b5a8:
	mul	fp, r9, fp
	sub	r0, r4, r1, asr #29
	cmp	r0, ip
	beq	.L_0813b650
.L_0813b5b8:
	subs	r0, r4, ip
	beq	.L_0813b650
	rsbmi	r0, r0, #0
	and	r2, r5, #0xff00
	rsbmi	r2, r2, #0x10000
	asrs	ip, r9, #16
	mlane	r7, r2, ip, r7
	lsl	ip, r9, #16
	asrs	ip, ip, #16
	mlane	r8, r2, ip, r8
	lsr	ip, r7, #16
	add	ip, fp, ip, lsl #16
	add	ip, ip, r8, lsr #16
	add	r7, pc, #0x194
	ldm	r7, {r7, r8, fp}
	and	r2, r3, #7
	add	r2, r7, r2, lsl #3
	add	r2, r2, sl, lsl #9
	bic	r7, r4, #7
	orr	r7, r7, r4, lsl #29
	tst	r0, #1
	bne	0x813b62c
	.inst	0xe00ba00c
	.inst	0xe18aa02c
	add	ip, ip, r9
	ldrb	sl, [r8, sl]
	strb	sl, [r2, r7, ror #29]
	adds	r7, r7, r1
	subcc	r7, r7, #8
@ Zero words that the set-up code fills by DMA3 with a copy of the words before them.
AlchemyUncredited_0813b62c:
	.inst	0
	.inst	0
	.inst	0
	.inst	0
	.inst	0
	.inst	0
	.inst	0
AlchemyUncreditedEnd_0813b62c:
	subs	r0, r0, #2
	bgt	0x813b610
.L_0813b650:
	ldm	sp, {r2, r4, r7, r8, sl, fp, ip}
	add	r5, r5, r7
	add	r6, r6, r8
	add	r7, r2, sl
	add	r8, r4, fp
	stm	sp, {r7, r8}
	adds	r3, r3, #1
	cmp	r3, ip
	blt	.L_0813b55c
.L_0813b674:
	cmp	r3, #64
	bge	.L_0813b2fc
	sub	lr, pc, #0x390
	ldm	lr, {r6, ip}
	rsbs	r4, r3, ip, asr #16
	beq	.L_0813b2fc
	str	ip, [sp, #24]
	lsl	r6, r6, #16
	rsbs	r8, r6, ip, lsl #16
	rsbmi	r8, r8, #0
	ldr	fp, [pc, #224]
	ldr	r0, [fp, r4, lsl #2]
	cmp	r0, #0
	umullsne	lr, r8, r0, r8
	rsbs	r0, r6, ip, lsl #16
	rsbmi	r8, r8, #0
	str	r8, [sp, #12]
	movs	r1, r1
	submi	r6, r6, #0x10000
	b	.L_0813b53c
.L_0813b6c4:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	ldr	fp, [pc, #180]
	bic	fp, fp, #1
	str	fp, [pc, #172]
	mov	r3, #0x3000000
	ldr	r3, [r3, #96]
	ldr	fp, [r0, #4]
	ldr	r2, [r0, #16]
	ldr	r4, [r2, #4]
	add	lr, pc, #152
	stm	lr, {r3, r4}
	ldr	r1, [r0, #12]
	ldrb	r4, [r2]
	rsb	r5, r4, #32
	ldr	r3, [pc, #-240]
	orr	r3, r3, r5, lsl #7
	str	r3, [pc, #-248]
	rsb	r5, r4, #31
	ldr	r3, [pc, #-704]
	orr	r3, r3, r5, lsl #7
	str	r3, [pc, #-712]
	rsbs	r5, r4, #8
	ldr	r3, [pc, #-712]
	orr	r3, r3, r5, lsl #7
	str	r3, [pc, #-720]
	ldr	r3, [pc, #-536]
	orr	r3, r3, r5, lsl #7
	str	r3, [pc, #-544]
	ldr	r3, [pc, #-300]
	orr	r3, r3, r5, lsl #7
	orrne	r3, r3, #32
	str	r3, [pc, #-312]
	ldrb	r6, [r2, #1]
	mov	r5, #1
	rsb	r5, r5, r5, lsl r6
	lsl	r5, r5, r4
	str	r5, [pc, #52]
	ldr	r3, [r0, #24]
	str	r3, [pc, #48]
	mov	r9, #0x4000000
	add	r9, r9, #212
	mov	r5, #7
	orr	r5, r5, #0x84000000
	sub	r3, pc, #0x168
	sub	r4, pc, #0x150
	stm	r9, {r3, r4, r5}
	ldr	r0, [r0, #8]
	b	.L_0813b304
AlchemyUncredited_0813b784:
	.4byte	0x080028a9
AlchemyUncreditedEnd_0813b784:
AlchemyUncredited_0813b788:
	.4byte	0x00000000
	.4byte	0x00000000
	.4byte	0x00000000
	.4byte	0x00000000
AlchemyUncreditedEnd_0813b788:
.L_0813b798:
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
