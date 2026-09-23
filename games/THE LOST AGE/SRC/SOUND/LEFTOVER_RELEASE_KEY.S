@ A copy of the sound driver's end-of-tie command in the leftover block of
@ LEFTOVER_HANDLE_NOTE.S. No bl reaches it and no literal pool names it. The
@ leaf saves r4 and r5 without lr (push {r4, r5} ... bx lr). These 64 bytes
@ equal TBS SOUND/DRIVER/RELEASE_KEY.S (0x080fa16c) and TLA RELEASE_KEY.S
@ (0x081c1e08) byte for byte.
@ credit: library — TBS/TLA common sound driver end-of-tie command routine
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x081c0398
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
