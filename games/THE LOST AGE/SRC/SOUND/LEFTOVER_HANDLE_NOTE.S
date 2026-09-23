@ A copy of the sound driver's note start in a leftover block of the TLA
@ ROM. The only bl to it is at 0x081c0152, in a code fragment that begins at
@ 0x081c00b0 without an entry of its own (it branches back to 0x081bff90 and
@ 0x081bff98, which hold zero bytes), and no literal pool names it (the one
@ ROM word equal to its address, at 0x08905964, lies outside the code). Its
@ own calls follow that stale layout: the stereo-volume call lands on
@ 0x081c0168 and the bx r3 call on 0x081c0114, both inside that fragment. It
@ saves the high registers in ascending order, restores all eight through
@ one pop {r0-r7} and reuses flags without a compare. These 512 bytes equal
@ TBS SOUND/DRIVER/HANDLE_NOTE.S (0x080f9f6c) apart from its last literal,
@ which is zero here where TBS has 0x080fba14; its bl offsets are the same
@ as there, which is why its calls land in that fragment. The zero halfword
@ before its literal pool is alignment and carries no credit.
@ credit: library — TBS/TLA common sound driver note-start routine
.syntax unified
	.set SoundDriver_CallViaR3, 0x081c0114
	.thumb
	.set AlchemyLoadAddress, 0x081c0198
	.global MusicTrack_HandleNoteLeftover
	.thumb_func
MusicTrack_HandleNoteLeftover:
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
	bcs.n	.L_081c01de
	strb	r0, [r5, #5]
	adds	r3, #1
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_081c01dc
	strb	r0, [r5, #6]
	adds	r3, #1
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_081c01dc
	ldrb	r1, [r5, #4]
	adds	r1, r1, r0
	strb	r1, [r5, #4]
	adds	r3, #1
.L_081c01dc:
	str	r3, [r5, #64]
.L_081c01de:
	movs	r0, #0
	str	r0, [sp, #20]
	adds	r4, r5, #0
	adds	r4, #36
	ldrb	r2, [r4, #0]
	movs	r0, #192
	tst	r0, r2
	beq.n	.L_081c0230
	ldrb	r3, [r5, #5]
	movs	r0, #64
	tst	r0, r2
	beq.n	.L_081c01fe
	ldr	r1, [r5, #44]
	adds	r1, r1, r3
	ldrb	r0, [r1, #0]
	b.n	.L_081c0200
.L_081c01fe:
	adds	r0, r3, #0
.L_081c0200:
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
	beq.n	.L_081c0218
	b.n	.L_081c037e
.L_081c0218:
	movs	r0, #128
	tst	r0, r2
	beq.n	.L_081c0234
	ldrb	r1, [r6, #3]
	movs	r0, #128
	tst	r0, r1
	beq.n	.L_081c022c
	subs	r1, #192
	lsls	r1, r1, #1
	str	r1, [sp, #20]
.L_081c022c:
	ldrb	r3, [r6, #1]
	b.n	.L_081c0234
.L_081c0230:
	mov	r9, r4
	ldrb	r3, [r5, #5]
.L_081c0234:
	str	r3, [sp, #8]
	ldr	r6, [sp, #0]
	ldrb	r1, [r6, #9]
	ldrb	r0, [r5, #29]
	adds	r0, r0, r1
	cmp	r0, #255
	bls.n	.L_081c0244
	movs	r0, #255
.L_081c0244:
	str	r0, [sp, #16]
	mov	r6, r9
	ldrb	r0, [r6, #0]
	movs	r6, #7
	ands	r6, r0
	str	r6, [sp, #12]
	beq.n	.L_081c0284
	ldr	r0, [sp, #4]
	ldr	r4, [r0, #28]
	cmp	r4, #0
	bne.n	.L_081c025c
	b.n	.L_081c037e
.L_081c025c:
	subs	r6, #1
	lsls	r0, r6, #6
	adds	r4, r4, r0
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_081c02d8
	movs	r0, #64
	tst	r0, r1
	bne.n	.L_081c02d8
	ldrb	r1, [r4, #19]
	ldr	r0, [sp, #16]
	cmp	r1, r0
	bcc.n	.L_081c02d8
	beq.n	.L_081c027c
	b.n	.L_081c037e
.L_081c027c:
	ldr	r0, [r4, #44]
	cmp	r0, r5
	bcs.n	.L_081c02d8
	b.n	.L_081c037e
.L_081c0284:
	ldr	r6, [sp, #16]
	adds	r7, r5, #0
	movs	r2, #0
	mov	r8, r2
	ldr	r4, [sp, #4]
	ldrb	r3, [r4, #6]
	adds	r4, #80
.L_081c0292:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_081c02d8
	movs	r0, #64
	tst	r0, r1
	beq.n	.L_081c02ac
	cmp	r2, #0
	bne.n	.L_081c02b0
	adds	r2, #1
	ldrb	r6, [r4, #19]
	ldr	r7, [r4, #44]
	b.n	.L_081c02ca
.L_081c02ac:
	cmp	r2, #0
	bne.n	.L_081c02cc
.L_081c02b0:
	ldrb	r0, [r4, #19]
	cmp	r0, r6
	bcs.n	.L_081c02bc
	adds	r6, r0, #0
	ldr	r7, [r4, #44]
	b.n	.L_081c02ca
.L_081c02bc:
	bhi.n	.L_081c02cc
	ldr	r0, [r4, #44]
	cmp	r0, r7
	bls.n	.L_081c02c8
	adds	r7, r0, #0
	b.n	.L_081c02ca
.L_081c02c8:
	bcc.n	.L_081c02cc
.L_081c02ca:
	mov	r8, r4
.L_081c02cc:
	adds	r4, #64
	subs	r3, #1
	bgt.n	.L_081c0292
	mov	r4, r8
	cmp	r4, #0
	beq.n	.L_081c037e
.L_081c02d8:
	adds	r0, r4, #0
	bl	Func_081c08a4
	movs	r1, #0
	str	r1, [r4, #48]
	ldr	r3, [r5, #32]
	str	r3, [r4, #52]
	cmp	r3, #0
	beq.n	.L_081c02ec
	str	r4, [r3, #48]
.L_081c02ec:
	str	r4, [r5, #32]
	str	r5, [r4, #44]
	ldrb	r0, [r5, #27]
	strb	r0, [r5, #28]
	cmp	r0, r1
	beq.n	.L_081c02fe
	adds	r1, r5, #0
	bl	Func_081c03d8
.L_081c02fe:
	ldr	r0, [sp, #0]
	adds	r1, r5, #0
	bl	Func_081c0e70
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
	bl	Func_081c0168
	ldrb	r1, [r4, #8]
	movs	r0, #8
	ldrsb	r0, [r5, r0]
	adds	r3, r1, r0
	bpl.n	.L_081c0338
	movs	r3, #0
.L_081c0338:
	ldr	r6, [sp, #12]
	cmp	r6, #0
	beq.n	.L_081c0366
	mov	r6, r9
	ldrb	r0, [r6, #2]
	strb	r0, [r4, #30]
	ldrb	r1, [r6, #3]
	movs	r0, #128
	tst	r0, r1
	bne.n	.L_081c0352
	movs	r0, #112
	tst	r0, r1
	bne.n	.L_081c0354
.L_081c0352:
	movs	r1, #8
.L_081c0354:
	strb	r1, [r4, #31]
	ldrb	r2, [r5, #9]
	adds	r1, r3, #0
	ldr	r0, [sp, #12]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #48]
	bl	SoundDriver_CallViaR3
	b.n	.L_081c0370
.L_081c0366:
	ldrb	r2, [r5, #9]
	adds	r1, r3, #0
	adds	r0, r7, #0
	bl	Func_081c0428
.L_081c0370:
	str	r0, [r4, #32]
	movs	r0, #128
	strb	r0, [r4, #0]
	ldrb	r1, [r5, #0]
	movs	r0, #240
	ands	r0, r1
	strb	r0, [r5, #0]
.L_081c037e:
	add	sp, #24
	pop	{r0, r1, r2, r3, r4, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	mov	fp, r3
	pop	{r0}
	bx	r0
AlchemyUncredited_081c038e:
	.2byte	0x0000
AlchemyUncreditedEnd_081c038e:
	.4byte	0x03007ff0
	.4byte	0x00000000
