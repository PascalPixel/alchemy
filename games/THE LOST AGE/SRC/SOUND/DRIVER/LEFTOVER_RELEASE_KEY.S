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
