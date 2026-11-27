@ Why assembly: a copy the ROM keeps of a function it also links, stopping
@ short of its end: its 232 bytes are those of the MPlayMain in
@ games/common/sdk/sound/clear_buffer.s, which this game keeps unreferenced,
@ from .L17 up to its final bx r3, and its bl and branches keep that
@ function's offsets. It keeps r11 as an ordinary register (mov r4, r11) and
@ restores its callee saves by popping them into the low registers, as
@ MPlayMain does.
@ The tail of an older copy of MusicPlayer2000's MPlayMain, kept in the
@ cartridge beside the leftover ply_note: it starts in the middle of the body and
@ stops short of its return, and its two backward branches land outside it. Only
@ the leftover ply_note refers to it.
@ Why assembly: a copy the ROM keeps of a function it also links: its bytes
@ are those of the ply_note in clear_buffer.s, which this game keeps
@ unreferenced, but for its last pool word, left 0, and its six bl keep that
@ function's offsets, so from here they land elsewhere, the first on
@ Func_081c08a4 inside ply_note.
@ Why assembly: push saves r4/r5 without lr; its bytes are ply_endtie's
@ (games/common/sdk/sound/clear_buffer.s).
@ Why assembly: a copy the ROM keeps of a function it also links: its 28 bytes
@ are clear_modM's (games/common/sdk/sound/clear_buffer.s).
@ Why assembly: MusicTrack_ReadCommandByteLeftover takes track in r1 and
@ returns the decoded byte in r3 while updating r2/r3, rather than a standard
@ r0 return. Its bytes are ld_r3_tp_adr_i_unchecked's
@ (games/common/sdk/sound/clear_buffer.s).
@ Why MusicTrack_SetLfoSpeedFromCommandLeftover to
@ MusicTrack_SetModulationFromCommandLeftover are assembly:
@ Both nested-call entries return through ip saved from lr rather than a
@ procedure-standard return: mov ip, lr before the bl, bx ip after it. Their
@ bytes are ply_lfos's and ply_mod's (games/common/sdk/sound/clear_buffer.s).
@ A copy of MidiKeyToFreq (games/common/sdk/sound/sound.c) that the cartridge
@ keeps 0x1a70 bytes before it, with the leftover functions under driver/,
@ which alone call it.
@ Its bytes are its twin's but for the two pool words, 0 where the twin's hold
@ the pitch code and frequency step tables' addresses. Its calls keep the
@ twin's offsets, so they land 0x1a70 bytes before the twin's umul3232H32, in
@ the zero fill before the sound module.
@ Why assembly: a copy of a function the ROM also links, its calls keeping
@ that function's offsets and its pool words never filled in.
	.section .text,"ax",%progbits
	.balign 2

.syntax unified
	.thumb
	mov	r3, r10
	mov	r4, r11
	subs	r6, #1
	ble.n	.L_081c00c0
	movs	r0, #80
	adds	r5, r5, r0
	lsls	r3, r3, #1
	b.n	. - 294
.L_081c00c0:
	ldr	r0, [r7, #12]
	adds	r0, #1
	str	r0, [r7, #12]
	cmp	r4, #0
	bne.n	.L_081c00d2
	movs	r0, #128
	lsls	r0, r0, #24
	str	r0, [r7, #4]
	b.n	.L_081c0188
.L_081c00d2:
	str	r4, [r7, #4]
	ldrh	r0, [r7, #34]
	subs	r0, #150
	strh	r0, [r7, #34]
	cmp	r0, #150
	bcc.n	.L_081c00e0
	b.n	. - 334
.L_081c00e0:
	ldrb	r2, [r7, #8]
	ldr	r5, [r7, #44]
.L_081c00e4:
	ldrb	r0, [r5, #0]
	movs	r1, #128
	tst	r1, r0
	beq.n	.L_081c017e
	movs	r1, #15
	tst	r1, r0
	beq.n	.L_081c017e
	mov	r9, r2
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sySndUpdateBGMRamps + 0xc4
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L_081c0174
.L_081c0102:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	bne.n	.L_081c0112
	adds	r0, r4, #0
	bl	Func_081c08a4 + 0x84
	b.n	.L_081c016e
.L_081c0112:
	ldrb	r0, [r4, #1]
	.global LeftoverSoundDriver_StaleCallTarget
LeftoverSoundDriver_StaleCallTarget:
	movs	r6, #7
	ands	r6, r0
	ldrb	r3, [r5, #0]
	movs	r0, #3
	tst	r0, r3
	beq.n	.L_081c0130
	bl	MusicTrack_HandleNoteLeftover + 0x54
	cmp	r6, #0
	beq.n	.L_081c0130
	ldrb	r0, [r4, #29]
	movs	r1, #1
	orrs	r0, r1
	strb	r0, [r4, #29]
.L_081c0130:
	ldrb	r3, [r5, #0]
	movs	r0, #12
	tst	r0, r3
	beq.n	.L_081c016e
	ldrb	r1, [r4, #8]
	movs	r0, #8
	ldrsb	r0, [r5, r0]
	adds	r2, r1, r0
	bpl.n	.L_081c0144
	movs	r2, #0
.L_081c0144:
	cmp	r6, #0
	beq.n	.L_081c0162
	mov	r0, r8
	ldr	r3, [r0, #48]
	adds	r1, r2, #0
	ldrb	r2, [r5, #9]
	adds	r0, r6, #0
	bl	MusicTrack_HandleNoteLeftover
	str	r0, [r4, #32]
	ldrb	r0, [r4, #29]
	movs	r1, #2
	orrs	r0, r1
	strb	r0, [r4, #29]
	b.n	.L_081c016e
.L_081c0162:
	adds	r1, r2, #0
	ldrb	r2, [r5, #9]
	ldr	r0, [r4, #36]
	.global Func_081c0168
	.thumb_func
Func_081c0168:
	bl	MidiKeyToFreqWorkCopy + 0x84
	str	r0, [r4, #32]
.L_081c016e:
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L_081c0102
.L_081c0174:
	ldrb	r0, [r5, #0]
	movs	r1, #240
	ands	r0, r1
	strb	r0, [r5, #0]
	mov	r2, r9
.L_081c017e:
	subs	r2, #1
	ble.n	.L_081c0188
	movs	r0, #80
	adds	r5, r5, r0
	bgt.n	.L_081c00e4
.L_081c0188:
	ldr	r0, [pc, #24]
	str	r0, [r7, #52]
	pop	{r0, r1, r2, r3, r4, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	r10, r2
	mov	r11, r3
	pop	{r3}

	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
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
	bl	MusicTrack_ClearModulationStateLeftover
.L_081c02fe:
	ldr	r0, [sp, #0]
	adds	r1, r5, #0
	bl	sySndUpdateBGMRamps + 0x40
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
	bl	LeftoverSoundDriver_StaleCallTarget
	b.n	.L_081c0370
.L_081c0366:
	ldrb	r2, [r5, #9]
	adds	r1, r3, #0
	adds	r0, r7, #0
	bl	MidiKeyToFreqWorkCopy
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

	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global MusicTrack_ReleaseKeyLeftover
	.thumb_func
MusicTrack_ReleaseKeyLeftover:
	push	{r4, r5}
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	cmp	r3, #128
	bcs.n	.L_081c03aa
	strb	r3, [r1, #5]
	adds	r2, #1
	str	r2, [r1, #64]
	b.n	.L_081c03ac
.L_081c03aa:
	ldrb	r3, [r1, #5]
.L_081c03ac:
	ldr	r1, [r1, #32]
	cmp	r1, #0
	beq.n	.L_081c03d4
	movs	r4, #131
	movs	r5, #64
.L_081c03b6:
	ldrb	r2, [r1, #0]
	tst	r2, r4
	beq.n	.L_081c03ce
	tst	r2, r5
	bne.n	.L_081c03ce
	ldrb	r0, [r1, #17]
	cmp	r0, r3
	bne.n	.L_081c03ce
	movs	r0, #64
	orrs	r2, r0
	strb	r2, [r1, #0]
	b.n	.L_081c03d4
.L_081c03ce:
	ldr	r1, [r1, #52]
	cmp	r1, #0
	bne.n	.L_081c03b6
.L_081c03d4:
	pop	{r4, r5}
	bx	lr

	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global MusicTrack_ClearModulationStateLeftover
	.thumb_func
MusicTrack_ClearModulationStateLeftover:
	movs	r2, #0
	strb	r2, [r1, #22]
	strb	r2, [r1, #26]
	ldrb	r2, [r1, #24]
	cmp	r2, #0
	bne.n	.L_081c03e8
	movs	r2, #12
	b.n	.L_081c03ea
.L_081c03e8:
	movs	r2, #3
.L_081c03ea:
	ldrb	r3, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	lr
AlchemyUncredited_081c03f2:
	.2byte	0x0000
AlchemyUncreditedEnd_081c03f2:

	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global MusicTrack_ReadCommandByteLeftover
	.thumb_func
MusicTrack_ReadCommandByteLeftover:
	ldr	r2, [r1, #64]
	adds	r3, r2, #1
	str	r3, [r1, #64]
	ldrb	r3, [r2, #0]
	bx	lr
	.2byte	0x0000

	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global MusicTrack_SetLfoSpeedFromCommandLeftover
	.thumb_func
MusicTrack_SetLfoSpeedFromCommandLeftover:
	mov	ip, lr
	bl	MusicTrack_ReadCommandByteLeftover
	strb	r3, [r1, #25]
	cmp	r3, #0
	bne.n	.L_081c0410
	bl	MusicTrack_ClearModulationStateLeftover
.L_081c0410:
	bx	ip
AlchemyUncredited_081c0412:
	.2byte	0x0000
	.global MusicTrack_SetModulationFromCommandLeftover
	.thumb_func
MusicTrack_SetModulationFromCommandLeftover:
AlchemyUncreditedEnd_081c0412:
	mov	ip, lr
	bl	MusicTrack_ReadCommandByteLeftover
	strb	r3, [r1, #23]
	cmp	r3, #0
	bne.n	.L_081c0424
	bl	MusicTrack_ClearModulationStateLeftover
.L_081c0424:
	bx	ip
AlchemyUncredited_081c0426:
	.2byte	0x0000
AlchemyUncreditedEnd_081c0426:

	.section .text,"ax",%progbits
	.balign 2

.syntax unified
	.thumb
	.global MidiKeyToFreqWorkCopy
	.thumb_func
MidiKeyToFreqWorkCopy:
	push	{r4, r5, r6, r7, lr}
	mov	r12, r0
	lsls	r1, r1, #24
	lsrs	r6, r1, #24
	lsls	r7, r2, #24
	cmp	r6, #178
	bls	.L_081c043c
	movs	r6, #178
	movs	r7, #255
	lsls	r7, r7, #24
.L_081c043c:
	ldr	r3, .L_081c0484
	adds	r0, r6, r3
	ldrb	r5, [r0]
	ldr	r4, .L_081c0488
	movs	r2, #15
	adds	r0, r5, #0
	ands	r0, r2
	lsls	r0, r0, #2
	adds	r0, r0, r4
	lsrs	r1, r5, #4
	ldr	r5, [r0]
	lsrs	r5, r1
	adds	r0, r6, #1
	adds	r0, r0, r3
	ldrb	r1, [r0]
	adds	r0, r1, #0
	ands	r0, r2
	lsls	r0, r0, #2
	adds	r0, r0, r4
	lsrs	r1, r1, #4
	ldr	r0, [r0]
	lsrs	r0, r1
	mov	r1, r12
	ldr	r4, [r1, #4]
	subs	r0, r0, r5
	adds	r1, r7, #0
	bl	Stubs_Sound - 0x7f4	@ 0x081bf80c: the twin's call to umul3232H32
	adds	r1, r0, #0
	adds	r1, r5, r1
	adds	r0, r4, #0
	bl	Stubs_Sound - 0x7f4	@ 0x081bf80c: the twin's call to umul3232H32
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
.L_081c0484:
	.4byte	0	@ the pitch codes (ScaleTable)
.L_081c0488:
	.4byte	0	@ the frequency steps (FreqTable)
