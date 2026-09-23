@ Starts a note on a music track: reads the optional key, velocity and length
@ bytes, picks the instrument, takes a free or the quietest channel and sets
@ up its voice. It saves the high registers in ascending order and restores
@ all eight through one pop {r0-r7}, which neither approved compiler emits. It
@ passes the stereo-volume helper its inputs in r4 and r5, reuses comparison
@ flags across branch targets, and calls the key-to-frequency hook through the
@ shared bx r3 at 0x080f9ee8. Every TBS and TLA edition has this routine; the
@ others differ from this one only in the relocated address of its last
@ literal.
@ The zero halfword of alignment at 0x080fa162 never runs and carries no
@ credit.
@ credit: library — TBS/TLA common sound driver note-start routine
.syntax unified
	.set SoundDriver_CallViaR3, 0x080f9ee8
	.thumb
	.set AlchemyLoadAddress, 0x080f9f6c
	.global MusicTrack_HandleNote
	.thumb_func
MusicTrack_HandleNote:
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
	bcs.n	.L_080f9fb2
	strb	r0, [r5, #5]
	adds	r3, #1
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_080f9fb0
	strb	r0, [r5, #6]
	adds	r3, #1
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_080f9fb0
	ldrb	r1, [r5, #4]
	adds	r1, r1, r0
	strb	r1, [r5, #4]
	adds	r3, #1
.L_080f9fb0:
	str	r3, [r5, #64]
.L_080f9fb2:
	movs	r0, #0
	str	r0, [sp, #20]
	adds	r4, r5, #0
	adds	r4, #36
	ldrb	r2, [r4, #0]
	movs	r0, #192
	tst	r0, r2
	beq.n	.L_080fa004
	ldrb	r3, [r5, #5]
	movs	r0, #64
	tst	r0, r2
	beq.n	.L_080f9fd2
	ldr	r1, [r5, #44]
	adds	r1, r1, r3
	ldrb	r0, [r1, #0]
	b.n	.L_080f9fd4
.L_080f9fd2:
	adds	r0, r3, #0
.L_080f9fd4:
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
	beq.n	.L_080f9fec
	b.n	.L_080fa152
.L_080f9fec:
	movs	r0, #128
	tst	r0, r2
	beq.n	.L_080fa008
	ldrb	r1, [r6, #3]
	movs	r0, #128
	tst	r0, r1
	beq.n	.L_080fa000
	subs	r1, #192
	lsls	r1, r1, #1
	str	r1, [sp, #20]
.L_080fa000:
	ldrb	r3, [r6, #1]
	b.n	.L_080fa008
.L_080fa004:
	mov	r9, r4
	ldrb	r3, [r5, #5]
.L_080fa008:
	str	r3, [sp, #8]
	ldr	r6, [sp, #0]
	ldrb	r1, [r6, #9]
	ldrb	r0, [r5, #29]
	adds	r0, r0, r1
	cmp	r0, #255
	bls.n	.L_080fa018
	movs	r0, #255
.L_080fa018:
	str	r0, [sp, #16]
	mov	r6, r9
	ldrb	r0, [r6, #0]
	movs	r6, #7
	ands	r6, r0
	str	r6, [sp, #12]
	beq.n	.L_080fa058
	ldr	r0, [sp, #4]
	ldr	r4, [r0, #28]
	cmp	r4, #0
	bne.n	.L_080fa030
	b.n	.L_080fa152
.L_080fa030:
	subs	r6, #1
	lsls	r0, r6, #6
	adds	r4, r4, r0
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_080fa0ac
	movs	r0, #64
	tst	r0, r1
	bne.n	.L_080fa0ac
	ldrb	r1, [r4, #19]
	ldr	r0, [sp, #16]
	cmp	r1, r0
	bcc.n	.L_080fa0ac
	beq.n	.L_080fa050
	b.n	.L_080fa152
.L_080fa050:
	ldr	r0, [r4, #44]
	cmp	r0, r5
	bcs.n	.L_080fa0ac
	b.n	.L_080fa152
.L_080fa058:
	ldr	r6, [sp, #16]
	adds	r7, r5, #0
	movs	r2, #0
	mov	r8, r2
	ldr	r4, [sp, #4]
	ldrb	r3, [r4, #6]
	adds	r4, #80
.L_080fa066:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_080fa0ac
	movs	r0, #64
	tst	r0, r1
	beq.n	.L_080fa080
	cmp	r2, #0
	bne.n	.L_080fa084
	adds	r2, #1
	ldrb	r6, [r4, #19]
	ldr	r7, [r4, #44]
	b.n	.L_080fa09e
.L_080fa080:
	cmp	r2, #0
	bne.n	.L_080fa0a0
.L_080fa084:
	ldrb	r0, [r4, #19]
	cmp	r0, r6
	bcs.n	.L_080fa090
	adds	r6, r0, #0
	ldr	r7, [r4, #44]
	b.n	.L_080fa09e
.L_080fa090:
	bhi.n	.L_080fa0a0
	ldr	r0, [r4, #44]
	cmp	r0, r7
	bls.n	.L_080fa09c
	adds	r7, r0, #0
	b.n	.L_080fa09e
.L_080fa09c:
	bcc.n	.L_080fa0a0
.L_080fa09e:
	mov	r8, r4
.L_080fa0a0:
	adds	r4, #64
	subs	r3, #1
	bgt.n	.L_080fa066
	mov	r4, r8
	cmp	r4, #0
	beq.n	.L_080fa152
.L_080fa0ac:
	adds	r0, r4, #0
	bl	Func_080fa678
	movs	r1, #0
	str	r1, [r4, #48]
	ldr	r3, [r5, #32]
	str	r3, [r4, #52]
	cmp	r3, #0
	beq.n	.L_080fa0c0
	str	r4, [r3, #48]
.L_080fa0c0:
	str	r4, [r5, #32]
	str	r5, [r4, #44]
	ldrb	r0, [r5, #27]
	strb	r0, [r5, #28]
	cmp	r0, r1
	beq.n	.L_080fa0d2
	adds	r1, r5, #0
	bl	Func_080fa1ac
.L_080fa0d2:
	ldr	r0, [sp, #0]
	adds	r1, r5, #0
	bl	Func_080fac44
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
	bl	Func_080f9f3c
	ldrb	r1, [r4, #8]
	movs	r0, #8
	ldrsb	r0, [r5, r0]
	adds	r3, r1, r0
	bpl.n	.L_080fa10c
	movs	r3, #0
.L_080fa10c:
	ldr	r6, [sp, #12]
	cmp	r6, #0
	beq.n	.L_080fa13a
	mov	r6, r9
	ldrb	r0, [r6, #2]
	strb	r0, [r4, #30]
	ldrb	r1, [r6, #3]
	movs	r0, #128
	tst	r0, r1
	bne.n	.L_080fa126
	movs	r0, #112
	tst	r0, r1
	bne.n	.L_080fa128
.L_080fa126:
	movs	r1, #8
.L_080fa128:
	strb	r1, [r4, #31]
	ldrb	r2, [r5, #9]
	adds	r1, r3, #0
	ldr	r0, [sp, #12]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #48]
	bl	SoundDriver_CallViaR3
	b.n	.L_080fa144
.L_080fa13a:
	ldrb	r2, [r5, #9]
	adds	r1, r3, #0
	adds	r0, r7, #0
	bl	Func_080fa1fc
.L_080fa144:
	str	r0, [r4, #32]
	movs	r0, #128
	strb	r0, [r4, #0]
	ldrb	r1, [r5, #0]
	movs	r0, #240
	ands	r0, r1
	strb	r0, [r5, #0]
.L_080fa152:
	add	sp, #24
	pop	{r0, r1, r2, r3, r4, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	mov	fp, r3
	pop	{r0}
	bx	r0
AlchemyUncredited_080fa162:
	.2byte	0x0000
AlchemyUncreditedEnd_080fa162:
	.4byte	0x03007ff0
	.4byte	0x080fba14
