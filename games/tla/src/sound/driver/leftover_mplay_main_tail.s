@ Why assembly: a copy the ROM keeps of a routine it also links, stopping
@ short of its end: its 232 bytes are SoundDriver_Update's from .L_081c1a9c
@ (update.s) up to its final bx r3, and its bl and branches keep that
@ routine's offsets. It keeps r11 as an ordinary register (mov r4, r11) and
@ restores its callee saves by popping them into the low registers, as
@ MPlayMain does.
@ The tail of an older copy of MusicPlayer2000's MPlayMain, kept in the
@ cartridge beside the leftover ply_note: it starts in the middle of the body and
@ stops short of its return, and its two backward branches land outside it. Only
@ the leftover ply_note refers to it.

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
	bl	Sound_UpdateBgmRamps + 0xc4
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
	bl	Func_081c0428 + 0x84
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
