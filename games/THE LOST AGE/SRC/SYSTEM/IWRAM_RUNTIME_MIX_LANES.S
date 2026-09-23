@ ARM part of the per-channel mixing in TLA's resident runtime. It picks a
@ six-word interpolation template of IWRAM_RUNTIME_MIX_CHANNEL.S
@ (add r0, pc, #0x1ac), and when the choice differs from the one recorded
@ in the state word at 0x03000af8 (IWRAM_RUNTIME_MIX_STATE.S; ldr r2,
@ [pc, #404]; str r0, [pc, #392]) it copies the template into the eight
@ lanes of its own sample loop (add lr, pc, #52; stm lr, {...}). The 48
@ words 0xfedcba98 that stand in those lanes in this image never run: the
@ state word starts at zero, so the first call fills every lane before the
@ loop runs, and the instructions that run there are copies of the credited
@ templates. Those placeholders carry no credit, and neither do the three
@ words after the routine where it saves r2, sl and sp before moving the
@ stack (add r9, pc, #0x1d8; stm r9, {r2, sl, sp}) and restores them
@ (add ip, pc, #4; ldm ip, {r2, r3, sp}); the stack-floor word 0x03007900
@ after them is the literal it loads (ldr sl, [pc, #500]). Rewriting its
@ own loop is beyond both approved compilers.
@ credit: handwritten — TLA IWRAM runtime sound mixer
.syntax unified
	.set	.L_03000cf4, 0x03000cf4
	.set	.L_03000c1c, 0x03000c1c
	.set	.L_03000eec, 0x03000eec
	.set	.L_03000b44, 0x03000b44
	.set	.L_03000b94, 0x03000b94
	.thumb
	.set AlchemyLoadAddress, 0x08000e38
	.set AlchemyRunAddress, 0x03000880
	.arm
	mov	r8, #0x210
	orrs	fp, sl, fp, lsl #16
	beq	.L_03000cf4
	ldr	r6, [pc, #1160]
	add	r6, r6, #1
	str	r6, [pc, #1152]
	tst	r0, #8
	bne	.L_03000c1c
	push	{r4, r9, ip}
	movs	r2, r2
	orreq	r0, r0, #64
	strbeq	r0, [r4, #1]
	add	r4, r4, #28
	ldm	r4, {r7, lr}
	mul	r4, ip, lr
	ldrsb	r6, [r3], #1
	ldrsb	ip, [r3]
	ands	r0, r0, #64
	bne	.L_03000eec
	sub	ip, ip, r6
	lsrs	fp, fp, #1
	adc	fp, fp, #0x8000
	bic	fp, fp, #0xff00
	mov	r1, r7
	umlal	r1, r0, r4, r8
	lsr	r1, r1, #23
	orr	r0, r1, r0, lsl #9
	cmp	r2, r0
	ble	.L_03000b44
	sub	r2, r2, r0
	ldr	sl, [pc, #500]
	add	sl, sl, r0
	cmp	sl, sp
	add	sl, r3, r0
	add	r9, pc, #0x1d8
	stm	r9, {r2, sl, sp}
	cmpcc	r0, #0x400
	bcs	.L_03000940
	bic	r1, r3, #3
	mov	r9, #0x4000000
	add	r9, r9, #212
	add	r0, r0, #7
	lsr	r0, r0, #2
	sub	sp, sp, r0, lsl #2
	and	r3, r3, #3
	add	r3, r3, sp
	orr	lr, r0, #0x84000000
	stm	r9, {r1, sp, lr}
.L_03000940:
	subs	r4, r4, #0x800000
	lslpl	fp, fp, #1
	add	r0, pc, #0x1ac
	addpl	r0, r0, #24
	subspl	r4, r4, #0x800000
	addpl	r0, r0, #24
	addpl	r4, r4, #0x800000
	ldr	r2, [pc, #404]
	cmp	r0, r2
	beq	.L_030009a0
	str	r0, [pc, #392]
	ldm	r0, {r0, r1, r2, r8, r9, sl}
	add	lr, pc, #52
.L_03000974:
	stm	lr, {r0, r1}
	add	lr, lr, #152
	stm	lr, {r0, r1}
	sub	lr, lr, #140
	stm	lr, {r2, r8, r9, sl}
	add	lr, lr, #152
	stm	lr, {r2, r8, r9, sl}
	sub	lr, lr, #128
	adds	r5, r5, #0x40000000
	bcc	.L_03000974
	mov	r8, #0x210
.L_030009a0:
	mov	r2, #0xff000000
.L_030009a4:
	ldm	r5, {r0, r1, sl, lr}
	mul	r9, r7, ip
AlchemyUncredited_08000f64:
	.inst	0xfedcba98
	.inst	0xfedcba98
AlchemyUncreditedEnd_08000f64:
	mlane	r0, fp, r9, r0
AlchemyUncredited_08000f70:
	.inst	0xfedcba98
	.inst	0xfedcba98
	.inst	0xfedcba98
	.inst	0xfedcba98
AlchemyUncreditedEnd_08000f70:
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
AlchemyUncredited_08000f88:
	.inst	0xfedcba98
	.inst	0xfedcba98
AlchemyUncreditedEnd_08000f88:
	mlane	r1, fp, r9, r1
AlchemyUncredited_08000f94:
	.inst	0xfedcba98
	.inst	0xfedcba98
	.inst	0xfedcba98
	.inst	0xfedcba98
AlchemyUncreditedEnd_08000f94:
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
AlchemyUncredited_08000fac:
	.inst	0xfedcba98
	.inst	0xfedcba98
AlchemyUncreditedEnd_08000fac:
	mlane	sl, fp, r9, sl
AlchemyUncredited_08000fb8:
	.inst	0xfedcba98
	.inst	0xfedcba98
	.inst	0xfedcba98
	.inst	0xfedcba98
AlchemyUncreditedEnd_08000fb8:
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
AlchemyUncredited_08000fd0:
	.inst	0xfedcba98
	.inst	0xfedcba98
AlchemyUncreditedEnd_08000fd0:
	mlane	lr, fp, r9, lr
AlchemyUncredited_08000fdc:
	.inst	0xfedcba98
	.inst	0xfedcba98
	.inst	0xfedcba98
	.inst	0xfedcba98
AlchemyUncreditedEnd_08000fdc:
	bic	r7, r7, r2, asr #1
	stmia	r5!, {r0, r1, sl, lr}
	ldm	r5, {r0, r1, sl, lr}
	mulne	r9, r7, ip
AlchemyUncredited_08000ffc:
	.inst	0xfedcba98
	.inst	0xfedcba98
AlchemyUncreditedEnd_08000ffc:
	mlane	r0, fp, r9, r0
AlchemyUncredited_08001008:
	.inst	0xfedcba98
	.inst	0xfedcba98
	.inst	0xfedcba98
	.inst	0xfedcba98
AlchemyUncreditedEnd_08001008:
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
AlchemyUncredited_08001020:
	.inst	0xfedcba98
	.inst	0xfedcba98
AlchemyUncreditedEnd_08001020:
	mlane	r1, fp, r9, r1
AlchemyUncredited_0800102c:
	.inst	0xfedcba98
	.inst	0xfedcba98
	.inst	0xfedcba98
	.inst	0xfedcba98
AlchemyUncreditedEnd_0800102c:
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
AlchemyUncredited_08001044:
	.inst	0xfedcba98
	.inst	0xfedcba98
AlchemyUncreditedEnd_08001044:
	mlane	sl, fp, r9, sl
AlchemyUncredited_08001050:
	.inst	0xfedcba98
	.inst	0xfedcba98
	.inst	0xfedcba98
	.inst	0xfedcba98
AlchemyUncreditedEnd_08001050:
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
AlchemyUncredited_08001068:
	.inst	0xfedcba98
	.inst	0xfedcba98
AlchemyUncreditedEnd_08001068:
	mlane	lr, fp, r9, lr
AlchemyUncredited_08001074:
	.inst	0xfedcba98
	.inst	0xfedcba98
	.inst	0xfedcba98
	.inst	0xfedcba98
AlchemyUncreditedEnd_08001074:
	bic	r7, r7, r2, asr #1
	stmia	r5!, {r0, r1, sl, lr}
	subs	r8, r8, #8
	bgt	.L_030009a4
	add	ip, pc, #4
	ldm	ip, {r2, r3, sp}
	b	.L_03000b94
AlchemyUncredited_080010a0:
	.4byte	0x00000000
	.4byte	0x00000000
	.4byte	0x00000000
AlchemyUncreditedEnd_080010a0:
	.4byte	0x03007900
