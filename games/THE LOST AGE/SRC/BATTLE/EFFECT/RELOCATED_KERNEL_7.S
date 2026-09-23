@ Battle-effect kernel 7, one of the bank described in RELOCATED_KERNELS.S,
@ from its 4-byte Thumb entry (bx pc) at 0x0813a444 to its return (pop
@ {r5-fp, lr}; bx lr) at 0x0813a91c. The entry branches to set-up code at
@ 0x0813a83c, which clears bit 0 of the kernel's closing literal 0x080028a9
@ at 0x0813a908, the address of the reciprocal table at 0x080028a8 (ldr fp,
@ [pc, #192]; bic fp, fp, #1; str fp, [pc, #184]), ORs shift amounts into
@ five of its own instructions (ldr r3, [pc, #-N]; orr r3, r3, r5, lsl #7;
@ str r3, [pc, #-N]), stores the four words after that literal and has DMA3
@ copy the eleven instruction words at 0x0813a768 over the eleven zero words
@ that follow them (0x0813a794; control 0x8400000b) before it enters its
@ loop, which then runs both copies and multiplies with umulls and umullsne.
@ The loop also saves two words just after the entry (0x0813a44c; sub r0,
@ pc, #236; stm r0, {r7, r8}). Rewriting its own instructions, storing to
@ PC-relative addresses and flag-setting long multiplies are beyond both
@ approved compilers. The entry, which is TLA's own, the two and four
@ variable words, the zero words, which never run as stored, and the
@ literal, a word rewritten at run time, carry no credit.
@ credit: handwritten — relocated self-modifying ARM kernel 7 of the battle-effect bank
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x0813a444
	.global Func_0813a444
	.thumb_func
Func_0813a444:
AlchemyUncredited_0813a444:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_0813a444:
	b	.L_0813a83c
AlchemyUncredited_0813a44c:
	.4byte	0
	.4byte	0
AlchemyUncreditedEnd_0813a44c:
.L_0813a454:
	add	sp, sp, #64
	pop	{r0, r1, fp}
.L_0813a45c:
	ldm	r0!, {r3, r9, sl}
	lsls	r5, r3, #8
	beq	.L_0813a91c
	and	r5, r3, #255
	ldr	r6, [r1, r5, lsl #3]
	and	r5, r3, #0xff00
	ldr	r7, [r1, r5, lsr #5]
	and	r5, r3, #0xff0000
	ldr	r8, [r1, r5, lsr #13]
	tst	fp, #3
	beq	.L_0813a4c4
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
	beq	.L_0813a45c
	eors	r3, r3, fp, lsl #31
	bpl	.L_0813a45c
.L_0813a4c4:
	cmp	r6, r7
	eorgt	r6, r6, r7
	eorgt	r7, r7, r6
	eorgt	r6, r6, r7
	rorgt	r9, r9, #16
	tst	r7, r8
	bmi	.L_0813a45c
	cmp	r7, r8
	ble	.L_0813a514
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
.L_0813a514:
	asrs	r3, r6, #23
	bgt	.L_0813a45c
	asrs	r3, r6, #16
	rsbs	r4, r3, r8, asr #16
	beq	.L_0813a45c
	push	{r0, r1, fp}
	sub	sp, sp, #64
	sub	r0, pc, #236
	stm	r0, {r7, r8}
	lsls	r6, r6, #16
	rsbs	r0, r6, r8, lsl #16
	rsbmi	r0, r0, #0
	ldr	r5, [pc, #956]
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
	ldr	r0, [pc, #884]
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
	beq	.L_0813a454
	ldr	r5, [pc, #772]
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
.L_0813a694:
	ldr	r4, [sp, #24]
	asr	r4, r4, #16
	cmp	r4, #128
	movge	r4, #128
	str	r4, [sp, #24]
	cmp	r3, r4
	beq	.L_0813a7ec
	ldm	sp, {r7, r8}
.L_0813a6b4:
	asrs	sl, r3, #3
	bmi	.L_0813a7c8
	asrs	r0, r6, #23
	mvnmi	ip, #0
	movpl	ip, #128
	asreq	ip, r6, #16
	asrs	r4, r5, #16
	asrs	fp, r4, #7
	beq	.L_0813a710
	bmi	.L_0813a6f0
	cmp	r6, #0x800000
	bge	.L_0813a7c8
	sub	fp, r4, #127
	mov	r4, #127
	b	.L_0813a700
.L_0813a6f0:
	movs	r6, r6
	bmi	.L_0813a7c8
	rsb	fp, r4, #0
	mov	r4, #0
.L_0813a700:
	mul	fp, r9, fp
	sub	r0, r4, r1, asr #29
	cmp	r0, ip
	beq	.L_0813a7c8
.L_0813a710:
	subs	r0, r4, ip
	beq	.L_0813a7c8
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
	add	r7, pc, #0x1c0
	ldm	r7, {r7, r8, fp}
	and	r2, r3, #7
	add	r2, r7, r2, lsl #3
	add	r2, r2, sl, lsl #10
	bic	r7, r4, #7
	orr	r7, r7, r4, lsl #29
	tst	r0, #1
	bne	0x813a794
	.inst	0xe00ba00c
	.inst	0xe18aa02c
	add	ip, ip, r9
	ldrb	sl, [r8, sl]
	subs	sl, sl, #0
	ble	.L_0813a78c
	ldrb	lr, [r2, r7, ror #29]
	cmp	lr, sl
	strblt	sl, [r2, r7, ror #29]
.L_0813a78c:
	adds	r7, r7, r1
	subcc	r7, r7, #8
@ Zero words that the set-up code fills by DMA3 with a copy of the words before them.
AlchemyUncredited_0813a794:
	.inst	0, 0, 0, 0, 0, 0, 0, 0
	.inst	0, 0, 0
AlchemyUncreditedEnd_0813a794:
	subs	r0, r0, #2
	bgt	0x813a768
.L_0813a7c8:
	ldm	sp, {r2, r4, r7, r8, sl, fp, ip}
	add	r5, r5, r7
	add	r6, r6, r8
	add	r7, r2, sl
	add	r8, r4, fp
	stm	sp, {r7, r8}
	adds	r3, r3, #1
	cmp	r3, ip
	blt	.L_0813a6b4
.L_0813a7ec:
	cmp	r3, #128
	bge	.L_0813a454
	sub	lr, pc, #0x3b0
	ldm	lr, {r6, ip}
	rsbs	r4, r3, ip, asr #16
	beq	.L_0813a454
	str	ip, [sp, #24]
	lsl	r6, r6, #16
	rsbs	r8, r6, ip, lsl #16
	rsbmi	r8, r8, #0
	ldr	fp, [pc, #236]
	ldr	r0, [fp, r4, lsl #2]
	cmp	r0, #0
	umullsne	lr, r8, r0, r8
	rsbs	r0, r6, ip, lsl #16
	rsbmi	r8, r8, #0
	str	r8, [sp, #12]
	movs	r1, r1
	submi	r6, r6, #0x10000
	b	.L_0813a694
.L_0813a83c:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	ldr	fp, [pc, #192]
	bic	fp, fp, #1
	str	fp, [pc, #184]
	mov	r3, #0x3000000
	ldr	r3, [r3, #96]
	ldr	fp, [r0, #4]
	ldr	r2, [r0, #16]
	ldr	r4, [r2, #4]
	add	lr, pc, #164
	stm	lr, {r3, r4}
	ldr	r1, [r0, #12]
	ldrb	r4, [r2]
	rsb	r5, r4, #32
	ldr	r3, [pc, #-272]
	orr	r3, r3, r5, lsl #7
	str	r3, [pc, #-280]
	rsb	r5, r4, #31
	ldr	r3, [pc, #-736]
	orr	r3, r3, r5, lsl #7
	str	r3, [pc, #-744]
	rsbs	r5, r4, #8
	ldr	r3, [pc, #-744]
	orr	r3, r3, r5, lsl #7
	str	r3, [pc, #-752]
	ldr	r3, [pc, #-568]
	orr	r3, r3, r5, lsl #7
	str	r3, [pc, #-576]
	ldr	r3, [pc, #-332]
	orr	r3, r3, r5, lsl #7
	orrne	r3, r3, #32
	str	r3, [pc, #-344]
	ldrb	r6, [r2, #1]
	mov	r5, #1
	rsb	r5, r5, r5, lsl r6
	lsl	r5, r5, r4
	str	r5, [pc, #64]
	ldr	r4, [r0, #20]
	rsbs	r4, r4, #0
	strb	r4, [pc, #-360]
	ldr	r3, [r0, #24]
	str	r3, [pc, #48]
	mov	r9, #0x4000000
	add	r9, r9, #212
	mov	r5, #11
	orr	r5, r5, #0x84000000
	sub	r3, pc, #0x194
	sub	r4, pc, #0x16c
	stm	r9, {r3, r4, r5}
	ldr	r0, [r0, #8]
	b	.L_0813a45c
AlchemyUncredited_0813a908:
	.4byte	0x080028a9
AlchemyUncreditedEnd_0813a908:
AlchemyUncredited_0813a90c:
	.4byte	0x00000000
	.4byte	0x00000000
	.4byte	0x00000000
	.4byte	0x00000000
AlchemyUncreditedEnd_0813a90c:
.L_0813a91c:
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
