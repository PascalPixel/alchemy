@ The TBS note-start routine as TLA still links it, though no literal pool
@ names it and no call reaches it (the driver uses HANDLE_NOTE.S). It calls
@ the stereo helper at 0x081c1bd8 through the private r4/r5 contract and the
@ bx r3 epilogue stub at 0x081c1b84, and reuses flags without a compare.
@ These 512 bytes equal TBS SOUND/DRIVER/HANDLE_NOTE.S (0x080f9f6c) apart from
@ one relocated literal word.
@ The zero halfword of alignment at 0x081c1dfe never runs and carries no
@ credit.
@ credit: library — TBS/TLA common sound driver note-start routine
.syntax unified
	.set SoundDriver_CallViaR3, 0x081c1b84
	.thumb
	.set AlchemyLoadAddress, 0x081c1c08
	.global MusicTrack_HandleNoteUnreferenced
	.thumb_func
MusicTrack_HandleNoteUnreferenced:
	push	{r4, r5, r6, r7, lr}
	mov	r4, r8
	mov	r5, r9
	mov	r6, sl
	mov	r7, fp
	push	{r4, r5, r6, r7}
	sub	sp, #24
	str	r1, [sp, #0]
	adds	r5, r2, #0
	ldr	r1, [pc, #484]
	ldr	r1, [r1, #0]
	str	r1, [sp, #4]
	ldr	r1, [pc, #480]
	adds	r0, r0, r1
	ldrb	r0, [r0, #0]
	strb	r0, [r5, #4]
	ldr	r3, [r5, #64]
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_081c1c4e
	strb	r0, [r5, #5]
	adds	r3, #1
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_081c1c4c
	strb	r0, [r5, #6]
	adds	r3, #1
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_081c1c4c
	ldrb	r1, [r5, #4]
	adds	r1, r1, r0
	strb	r1, [r5, #4]
	adds	r3, #1
.L_081c1c4c:
	str	r3, [r5, #64]
.L_081c1c4e:
	movs	r0, #0
	str	r0, [sp, #20]
	adds	r4, r5, #0
	adds	r4, #36
	ldrb	r2, [r4, #0]
	movs	r0, #192
	tst	r0, r2
	beq.n	.L_081c1ca0
	ldrb	r3, [r5, #5]
	movs	r0, #64
	tst	r0, r2
	beq.n	.L_081c1c6e
	ldr	r1, [r5, #44]
	adds	r1, r1, r3
	ldrb	r0, [r1, #0]
	b.n	.L_081c1c70
.L_081c1c6e:
	adds	r0, r3, #0
.L_081c1c70:
	lsls	r1, r0, #1
	adds	r1, r1, r0
	lsls	r1, r1, #2
	ldr	r0, [r5, #40]
	adds	r1, r1, r0
	mov	r9, r1
	mov	r6, r9
	ldrb	r1, [r6, #0]
	movs	r0, #192
	tst	r0, r1
	beq.n	.L_081c1c88
	b.n	.L_081c1dee
.L_081c1c88:
	movs	r0, #128
	tst	r0, r2
	beq.n	.L_081c1ca4
	ldrb	r1, [r6, #3]
	movs	r0, #128
	tst	r0, r1
	beq.n	.L_081c1c9c
	subs	r1, #192
	lsls	r1, r1, #1
	str	r1, [sp, #20]
.L_081c1c9c:
	ldrb	r3, [r6, #1]
	b.n	.L_081c1ca4
.L_081c1ca0:
	mov	r9, r4
	ldrb	r3, [r5, #5]
.L_081c1ca4:
	str	r3, [sp, #8]
	ldr	r6, [sp, #0]
	ldrb	r1, [r6, #9]
	ldrb	r0, [r5, #29]
	adds	r0, r0, r1
	cmp	r0, #255
	bls.n	.L_081c1cb4
	movs	r0, #255
.L_081c1cb4:
	str	r0, [sp, #16]
	mov	r6, r9
	ldrb	r0, [r6, #0]
	movs	r6, #7
	ands	r6, r0
	str	r6, [sp, #12]
	beq.n	.L_081c1cf4
	ldr	r0, [sp, #4]
	ldr	r4, [r0, #28]
	cmp	r4, #0
	bne.n	.L_081c1ccc
	b.n	.L_081c1dee
.L_081c1ccc:
	subs	r6, #1
	lsls	r0, r6, #6
	adds	r4, r4, r0
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_081c1d48
	movs	r0, #64
	tst	r0, r1
	bne.n	.L_081c1d48
	ldrb	r1, [r4, #19]
	ldr	r0, [sp, #16]
	cmp	r1, r0
	bcc.n	.L_081c1d48
	beq.n	.L_081c1cec
	b.n	.L_081c1dee
.L_081c1cec:
	ldr	r0, [r4, #44]
	cmp	r0, r5
	bcs.n	.L_081c1d48
	b.n	.L_081c1dee
.L_081c1cf4:
	ldr	r6, [sp, #16]
	adds	r7, r5, #0
	movs	r2, #0
	mov	r8, r2
	ldr	r4, [sp, #4]
	ldrb	r3, [r4, #6]
	adds	r4, #80
.L_081c1d02:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_081c1d48
	movs	r0, #64
	tst	r0, r1
	beq.n	.L_081c1d1c
	cmp	r2, #0
	bne.n	.L_081c1d20
	adds	r2, #1
	ldrb	r6, [r4, #19]
	ldr	r7, [r4, #44]
	b.n	.L_081c1d3a
.L_081c1d1c:
	cmp	r2, #0
	bne.n	.L_081c1d3c
.L_081c1d20:
	ldrb	r0, [r4, #19]
	cmp	r0, r6
	bcs.n	.L_081c1d2c
	adds	r6, r0, #0
	ldr	r7, [r4, #44]
	b.n	.L_081c1d3a
.L_081c1d2c:
	bhi.n	.L_081c1d3c
	ldr	r0, [r4, #44]
	cmp	r0, r7
	bls.n	.L_081c1d38
	adds	r7, r0, #0
	b.n	.L_081c1d3a
.L_081c1d38:
	bcc.n	.L_081c1d3c
.L_081c1d3a:
	mov	r8, r4
.L_081c1d3c:
	adds	r4, #64
	subs	r3, #1
	bgt.n	.L_081c1d02
	mov	r4, r8
	cmp	r4, #0
	beq.n	.L_081c1dee
.L_081c1d48:
	adds	r0, r4, #0
	bl	Func_081c2314
	movs	r1, #0
	str	r1, [r4, #48]
	ldr	r3, [r5, #32]
	str	r3, [r4, #52]
	cmp	r3, #0
	beq.n	.L_081c1d5c
	str	r4, [r3, #48]
.L_081c1d5c:
	str	r4, [r5, #32]
	str	r5, [r4, #44]
	ldrb	r0, [r5, #27]
	strb	r0, [r5, #28]
	cmp	r0, r1
	beq.n	.L_081c1d6e
	adds	r1, r5, #0
	bl	Func_081c1e48
.L_081c1d6e:
	ldr	r0, [sp, #0]
	adds	r1, r5, #0
	bl	Func_081c28e0
	ldr	r0, [r5, #4]
	str	r0, [r4, #16]
	ldr	r0, [sp, #16]
	strb	r0, [r4, #19]
	ldr	r0, [sp, #8]
	strb	r0, [r4, #8]
	ldr	r0, [sp, #20]
	strb	r0, [r4, #20]
	mov	r6, r9
	ldrb	r0, [r6, #0]
	strb	r0, [r4, #1]
	ldr	r7, [r6, #4]
	str	r7, [r4, #36]
	ldr	r0, [r6, #8]
	str	r0, [r4, #4]
	ldrh	r0, [r5, #30]
	strh	r0, [r4, #12]
	bl	Func_081c1bd8
	ldrb	r1, [r4, #8]
	movs	r0, #8
	ldrsb	r0, [r5, r0]
	adds	r3, r1, r0
	bpl.n	.L_081c1da8
	movs	r3, #0
.L_081c1da8:
	ldr	r6, [sp, #12]
	cmp	r6, #0
	beq.n	.L_081c1dd6
	mov	r6, r9
	ldrb	r0, [r6, #2]
	strb	r0, [r4, #30]
	ldrb	r1, [r6, #3]
	movs	r0, #128
	tst	r0, r1
	bne.n	.L_081c1dc2
	movs	r0, #112
	tst	r0, r1
	bne.n	.L_081c1dc4
.L_081c1dc2:
	movs	r1, #8
.L_081c1dc4:
	strb	r1, [r4, #31]
	ldrb	r2, [r5, #9]
	adds	r1, r3, #0
	ldr	r0, [sp, #12]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #48]
	bl	SoundDriver_CallViaR3
	b.n	.L_081c1de0
.L_081c1dd6:
	ldrb	r2, [r5, #9]
	adds	r1, r3, #0
	adds	r0, r7, #0
	bl	Func_081c1e98
.L_081c1de0:
	str	r0, [r4, #32]
	movs	r0, #128
	strb	r0, [r4, #0]
	ldrb	r1, [r5, #0]
	movs	r0, #240
	ands	r0, r1
	strb	r0, [r5, #0]
.L_081c1dee:
	add	sp, #24
	pop	{r0, r1, r2, r3, r4, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	mov	fp, r3
	pop	{r0}
	bx	r0
AlchemyUncredited_081c1dfe:
	.2byte	0x0000
AlchemyUncreditedEnd_081c1dfe:
	.4byte	0x03007ff0
	.4byte	0x081c36b0
