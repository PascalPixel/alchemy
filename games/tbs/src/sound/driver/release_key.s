.syntax unified
	.thumb
	.global MusicTrack_ReleaseKey
	.thumb_func
MusicTrack_ReleaseKey:
	push	{r4, r5}
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	cmp	r3, #128
	bcs.n	.L_080fa17e
	strb	r3, [r1, #5]
	adds	r2, #1
	str	r2, [r1, #64]
	b.n	.L_080fa180
.L_080fa17e:
	ldrb	r3, [r1, #5]
.L_080fa180:
	ldr	r1, [r1, #32]
	cmp	r1, #0
	beq.n	.L_080fa1a8
	movs	r4, #131
	movs	r5, #64
.L_080fa18a:
	ldrb	r2, [r1, #0]
	tst	r2, r4
	beq.n	.L_080fa1a2
	tst	r2, r5
	bne.n	.L_080fa1a2
	ldrb	r0, [r1, #17]
	cmp	r0, r3
	bne.n	.L_080fa1a2
	movs	r0, #64
	orrs	r2, r0
	strb	r2, [r1, #0]
	b.n	.L_080fa1a8
.L_080fa1a2:
	ldr	r1, [r1, #52]
	cmp	r1, #0
	bne.n	.L_080fa18a
.L_080fa1a8:
	pop	{r4, r5}
	bx	lr
