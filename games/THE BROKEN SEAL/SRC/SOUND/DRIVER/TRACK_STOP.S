@ Stops a music track: walks its channel list, switches every sounding channel
@ off through the driver's channel-off hook (SoundInfo+0x2c) and clears it.
@ It branches on the flags left by tst r0, r1 (0x080f9f00) and by
@ ands r0, r3; beq at 0x080f9f16 with no compare in between; both approved
@ compilers put a cmp before every conditional branch. The hook is reached by
@ bl to the bx r3 at 0x080f9ee8. Every TBS and TLA edition has these bytes.
@ The zero halfword of alignment at 0x080f9f36 never runs and carries no
@ credit.
@ credit: library — TBS/TLA common sound driver track-stop routine
.syntax unified
	.set SoundDriver_CallViaR3, 0x080f9ee8
	.thumb
	.set AlchemyLoadAddress, 0x080f9ef8
	.global MusicTrack_Stop
	.thumb_func
MusicTrack_Stop:
	push	{r4, r5, r6, lr}
	adds	r5, r1, #0
	ldrb	r1, [r5, #0]
	movs	r0, #128
	tst	r0, r1
	beq.n	.L_080f9f30
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L_080f9f2e
	movs	r6, #0
.L_080f9f0c:
	ldrb	r0, [r4, #0]
	cmp	r0, #0
	beq.n	.L_080f9f26
	ldrb	r0, [r4, #1]
	movs	r3, #7
	ands	r0, r3
	beq.n	.L_080f9f24
	ldr	r3, [pc, #28]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #44]
	bl	SoundDriver_CallViaR3
.L_080f9f24:
	strb	r6, [r4, #0]
.L_080f9f26:
	str	r6, [r4, #44]
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L_080f9f0c
.L_080f9f2e:
	str	r4, [r5, #32]
.L_080f9f30:
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
AlchemyUncredited_080f9f36:
	.2byte	0x0000
AlchemyUncreditedEnd_080f9f36:
	.4byte	0x03007ff0
