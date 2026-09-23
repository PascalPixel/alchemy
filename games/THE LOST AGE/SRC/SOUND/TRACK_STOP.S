@ Stops a music track: for each sounding channel it loads the channel-off
@ hook into r3 and reaches it by bl to the bx r3 that ends UPDATE.S
@ (0x081c1b84), leaving r1 and r2 unset, a private call contract no C call
@ has; movs r3, #7; ands r0, r3; beq branches on the flags of ands with no
@ compare. These 68 bytes equal TBS SOUND/DRIVER/TRACK_STOP.S (0x080f9ef8).
@ The zero halfword of alignment at 0x081c1bd2 never runs and carries no
@ credit.
@ credit: library — TBS/TLA common sound driver track-stop routine
.syntax unified
	.set SoundDriver_CallViaR3, 0x081c1b84
	.thumb
	.set AlchemyLoadAddress, 0x081c1b94
	.global MusicTrack_Stop
	.thumb_func
MusicTrack_Stop:
	push	{r4, r5, r6, lr}
	adds	r5, r1, #0
	ldrb	r1, [r5, #0]
	movs	r0, #128
	tst	r0, r1
	beq.n	.L_081c1bcc
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L_081c1bca
	movs	r6, #0
.L_081c1ba8:
	ldrb	r0, [r4, #0]
	cmp	r0, #0
	beq.n	.L_081c1bc2
	ldrb	r0, [r4, #1]
	movs	r3, #7
	ands	r0, r3
	beq.n	.L_081c1bc0
	ldr	r3, [pc, #28]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #44]
	bl	SoundDriver_CallViaR3
.L_081c1bc0:
	strb	r6, [r4, #0]
.L_081c1bc2:
	str	r6, [r4, #44]
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L_081c1ba8
.L_081c1bca:
	str	r4, [r5, #32]
.L_081c1bcc:
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
AlchemyUncredited_081c1bd2:
	.2byte	0x0000
AlchemyUncreditedEnd_081c1bd2:
	.4byte	0x03007ff0
