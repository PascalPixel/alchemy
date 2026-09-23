.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_080ad348, 0x080ad348
	.global Trade_CanOfferDjinn
	.global Func_080b0bb8
	.thumb_func
Trade_CanOfferDjinn:
Func_080b0bb8:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r6, r2, #0
	adds	r7, r0, #0
	bl	sub_08016ca4
	movs	r2, #140
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r0, r3]
	cmp	r3, #0
	beq.n	.L_080b0c72
	movs	r3, #142
	lsls	r3, r3, #1
	adds	r2, r5, r3
	ldrb	r3, [r0, r2]
	cmp	r3, #9
	bls.n	.L_080b0be2
	movs	r3, #10
	strb	r3, [r0, r2]
	b.n	.L_080b0c72
.L_080b0be2:
	lsls	r1, r5, #2
	adds	r3, r1, #0
	adds	r3, #248
	ldr	r3, [r0, r3]
	movs	r2, #1
	lsls	r2, r6
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b0c72
	movs	r4, #132
	lsls	r4, r4, #1
	adds	r3, r1, r4
	ldr	r3, [r0, r3]
	movs	r0, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080b0c74
	cmp	r7, #7
	bls.n	.L_080b0c0a
	movs	r0, #1
.L_080b0c0a:
	bl	sub_080ad348
	movs	r2, #148
	adds	r3, r0, #0
	lsls	r2, r2, #1
	adds	r1, r3, #0
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	movs	r4, #0
	adds	r1, #8
	cmp	r4, r3
	bge.n	.L_080b0c4c
	ldrb	r3, [r1, #0]
	cmp	r5, r3
	bne.n	.L_080b0c2e
	ldrb	r3, [r1, #1]
	cmp	r6, r3
	beq.n	.L_080b0c4c
.L_080b0c2e:
	movs	r2, #144
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	adds	r4, #1
	cmp	r4, r3
	bge.n	.L_080b0c4c
	lsls	r2, r4, #2
	ldrb	r3, [r1, r2]
	cmp	r5, r3
	bne.n	.L_080b0c2e
	adds	r3, r1, r2
	ldrb	r3, [r3, #1]
	cmp	r6, r3
	bne.n	.L_080b0c2e
.L_080b0c4c:
	movs	r2, #144
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	cmp	r4, r3
	beq.n	.L_080b0c6e
	lsls	r3, r4, #2
	adds	r3, r1, r3
	ldrb	r3, [r3, #3]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bgt.n	.L_080b0c72
	movs	r4, #2
	negs	r4, r4
	cmp	r3, r4
	beq.n	.L_080b0c72
.L_080b0c6e:
	movs	r0, #1
	b.n	.L_080b0c74
.L_080b0c72:
	movs	r0, #0
.L_080b0c74:
	pop	{r5, r6, r7, pc}
