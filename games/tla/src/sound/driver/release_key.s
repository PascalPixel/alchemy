.syntax unified
	.thumb
	.global ply_endtie
	.thumb_func
ply_endtie:
	push	{r4, r5}
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	cmp	r3, #128
	bcs.n	.L_081c1e1a
	strb	r3, [r1, #5]
	adds	r2, #1
	str	r2, [r1, #64]
	b.n	.L_081c1e1c
.L_081c1e1a:
	ldrb	r3, [r1, #5]
.L_081c1e1c:
	ldr	r1, [r1, #32]
	cmp	r1, #0
	beq.n	.L_081c1e44
	movs	r4, #131
	movs	r5, #64
.L_081c1e26:
	ldrb	r2, [r1, #0]
	tst	r2, r4
	beq.n	.L_081c1e3e
	tst	r2, r5
	bne.n	.L_081c1e3e
	ldrb	r0, [r1, #17]
	cmp	r0, r3
	bne.n	.L_081c1e3e
	movs	r0, #64
	orrs	r2, r0
	strb	r2, [r1, #0]
	b.n	.L_081c1e44
.L_081c1e3e:
	ldr	r1, [r1, #52]
	cmp	r1, #0
	bne.n	.L_081c1e26
.L_081c1e44:
	pop	{r4, r5}
	bx	lr
