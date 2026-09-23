@ Kernels 11 and 12 of the battle-effect bank described in
@ RELOCATED_KERNELS.S, each behind a 4-byte Thumb entry (bx pc). Kernel 11,
@ like kernels 1 to 3, clears bit 0 of its closing literal 0x080028a9
@ (0x0813b994), the address of the reciprocal table at 0x080028a8, at run
@ time (ldr fp, [pc, #484]; bic fp, fp, #1; str fp, [pc, #476]), loads it
@ again as that table's address and multiplies with the flag-setting
@ umulls. Kernel 12 saves r5-fp without lr (push {r5-fp}; pop {r5-fp};
@ bx lr); its last word, 0x0300049c, is the literal it loads
@ (ldr r4, [pc, #156]). GCC 2.96 never stores to a PC-relative address, has
@ no flag-setting long multiply and pushes lr with any register it saves.
@ The entries, which are TLA's own, and kernel 11's literal, a word
@ rewritten at run time, carry no credit.
@ credit: handwritten — relocated ARM kernels 11 and 12 of the battle-effect bank
.syntax unified
	.set AlchemyLoadAddress, 0x0813b7a0
	.thumb
	.global Func_0813b7a0
	.thumb_func
Func_0813b7a0:
AlchemyUncredited_0813b7a0:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_0813b7a0:
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
.L_0813b7cc:
	ldr	r3, [r0], #4
	lsls	r5, r3, #8
	beq	.L_0813b998
	add	fp, fp, #1
	and	r5, r3, #255
	ldr	r6, [r1, r5, lsl #3]
	and	r5, r3, #0xff00
	ldr	r7, [r1, r5, lsr #5]
	and	r5, r3, #0xff0000
	ldr	r8, [r1, r5, lsr #13]
	tst	r9, #3
	beq	.L_0813b838
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
	beq	.L_0813b7cc
	eors	r3, r3, r9, lsl #31
	bpl	.L_0813b7cc
.L_0813b838:
	cmp	r6, r7
	eorgt	r6, r6, r7
	eorgt	r7, r7, r6
	eorgt	r6, r6, r7
	cmp	r7, r8
	ble	.L_0813b86c
	eor	r7, r7, r8
	eor	r8, r8, r7
	eor	r7, r7, r8
	cmp	r6, r7
	eorgt	r6, r6, r7
	eorgt	r7, r7, r6
	eorgt	r6, r6, r7
.L_0813b86c:
	asrs	r3, r6, #22
	bgt	.L_0813b7cc
	asrs	r3, r6, #16
	rsbs	r4, r3, r8, asr #16
	beq	.L_0813b7cc
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
.L_0813b8b0:
	rsbs	r0, r3, r7, asr #16
	beq	.L_0813b97c
	rsbs	r9, r6, r7, lsl #16
	rsbmi	r9, r9, #0
	ldr	r4, [pc, #204]
	ldr	sl, [r4, r0, lsl #2]
	umulls	r4, r0, sl, r9
	moveq	r0, r9
	rsbs	r9, r6, r7, lsl #16
	rsbmi	r0, r0, #0
	cmp	r7, #0x400000
	movgt	r7, #0x400000
	bgt	.L_0813b974
.L_0813b8e4:
	asrs	r4, r3, #3
	bmi	.L_0813b968
	asrs	r9, r5, #16
	subs	sl, r9, r6, asr #16
	rsbmi	sl, sl, #0
	asrgt	r9, r6, #16
	asrs	r4, r9, #6
	bgt	.L_0813b968
	addmi	sl, sl, r9
	movmi	r9, #0
	adds	r4, r9, sl
	ble	.L_0813b968
	subs	r4, r4, #64
	subsgt	sl, sl, r4
	cmp	sl, #0
	ble	.L_0813b968
	asrs	r4, r3, #3
	and	r2, r3, #7
	lsls	r2, r2, #3
	orr	r2, r2, r4, lsl #9
	add	r2, r2, lr
	bic	r4, r9, #7
	orr	r9, r4, r9, lsl #29
	tst	sl, #1
	bne	.L_0813b954
.L_0813b948:
	strb	r1, [r2, r9, ror #29]
	adds	r9, r9, #0x20000000
	addcs	r9, r9, #8
.L_0813b954:
	strb	r1, [r2, r9, ror #29]
	adds	r9, r9, #0x20000000
	addcs	r9, r9, #8
	subs	sl, sl, #2
	bgt	.L_0813b948
.L_0813b968:
	add	r5, r5, ip
	add	r6, r6, r0
	add	r3, r3, #1
.L_0813b974:
	subs	r4, r3, r7, asr #16
	bne	.L_0813b8e4
.L_0813b97c:
	lsl	r6, r7, #16
	movs	r7, r8
	mov	r8, #0
	bne	.L_0813b8b0
	pop	{r0, r1, r9}
	b	.L_0813b7cc
AlchemyUncredited_0813b994:
	.4byte	0x080028a9
.L_0813b998:
AlchemyUncreditedEnd_0813b994:
	pop	{r5, r6, r7, r8, r9, sl, fp, ip, lr}
	bx	lr
	.thumb
	.global Func_0813b9a0
	.thumb_func
Func_0813b9a0:
AlchemyUncredited_0813b9a0:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_0813b9a0:
	push	{r5, r6, r7, r8, r9, sl, fp}
	ldr	r4, [pc, #156]
	ldm	r4, {r5, r6}
	add	r5, r5, #0x400000
	add	r6, r6, #0x400000
	stm	r4, {r5, r6}
	add	r4, r4, #12
	mov	fp, #0xff000000
	b	.L_0813ba28
.L_0813b9c8:
	ldr	r3, [r0], #4
	ldmdb	r4!, {r5, r6, r7, r8, r9, sl}
	and	ip, fp, r3, ror #24
	asrs	ip, ip, #24
	mlane	r8, r5, ip, r8
	mlane	r9, r6, ip, r9
	mlane	sl, r7, ip, sl
	ldmdb	r4!, {r5, r6, r7}
	and	ip, fp, r3, ror #16
	asrs	ip, ip, #24
	mlane	r8, r5, ip, r8
	mlane	r9, r6, ip, r9
	mlane	sl, r7, ip, sl
	ldmdb	r4!, {r5, r6, r7}
	and	ip, fp, r3, ror #8
	asrs	ip, ip, #24
	mlane	r8, r5, ip, r8
	mlane	r9, r6, ip, r9
	mlane	sl, r7, ip, sl
	and	r9, r9, fp, asr #8
	orr	r9, r9, r8, lsr #16
	lsr	sl, sl, #16
	stmia	r1!, {r9, sl}
	add	r4, r4, #48
.L_0813ba28:
	subs	r2, r2, #1
	bpl	.L_0813b9c8
	sub	r4, r4, #12
	ldm	r4, {r5, r6}
	sub	r5, r5, #0x400000
	sub	r6, r6, #0x400000
	stm	r4, {r5, r6}
	pop	{r5, r6, r7, r8, r9, sl, fp}
	bx	lr
	.4byte	0x0300049c
