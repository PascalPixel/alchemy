@ Channel-chain helpers of the MusicPlayer2000 driver's assembly object, which
@ runs unbroken from UMUL_HIGH.S (0x080f95e0) to COMMAND_BYTE_SETTERS.S
@ (0x080fa1fc). 0x080f9a30 unlinks a channel from its track's chain, holding
@ the track in r3 and the neighbours in r1 and r2 of the argument register;
@ old_agbcc copies the argument to r2 first. 0x080f9a50 releases every
@ channel of a track: it tests the channel state with movs r0, #0xc7;
@ tst r0, r1, and neither approved compiler has a Thumb tst pattern (both
@ emit and plus cmp for every spelling). SEQUENCE_COMMANDS.S reaches it
@ with b.n and the sound jump table at 0x080fb7a0 names it for its unused
@ commands and 0x080f9a30 as entry 34. TLA links the same bytes at
@ 0x081c16cc.
@ The zero halfword of alignment at 0x080f9a7e never runs and carries no
@ credit.
@ credit: library — MusicPlayer2000 sound driver assembly object, channel-chain clear and track release
.syntax unified
	.thumb
	.global SoundNote_Detach
	.global Func_080f9a30
	.thumb_func
SoundNote_Detach:
Func_080f9a30:
	ldr	r3, [r0, #44]
	cmp	r3, #0
	beq.n	.L_080f9a4e
	ldr	r1, [r0, #52]
	ldr	r2, [r0, #48]
	cmp	r2, #0
	beq.n	.L_080f9a42
	str	r1, [r2, #52]
	b.n	.L_080f9a44
.L_080f9a42:
	str	r1, [r3, #32]
.L_080f9a44:
	cmp	r1, #0
	beq.n	.L_080f9a4a
	str	r2, [r1, #48]
.L_080f9a4a:
	movs	r1, #0
	str	r1, [r0, #44]
.L_080f9a4e:
	bx	lr
	.global Func_080f9a50
	.thumb_func
Func_080f9a50:
	push	{r4, r5, lr}
	adds	r5, r1, #0
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L_080f9a74
.L_080f9a5a:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_080f9a68
	movs	r0, #64
	orrs	r1, r0
	strb	r1, [r4, #0]
.L_080f9a68:
	adds	r0, r4, #0
	bl	Func_080f9a30
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L_080f9a5a
.L_080f9a74:
	movs	r0, #0
	strb	r0, [r5, #0]
	pop	{r4, r5}
	pop	{r0}
	bx	r0
AlchemyUncredited_080f9a7e:
	.2byte	0x0000
AlchemyUncreditedEnd_080f9a7e:
