.syntax unified
	.thumb
	.global Overlay_0801e318
Overlay_0801e318:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #144]
	movs	r2, #30
	ldr	r5, [r3, #0]
	mov	r9, r2
	ldr	r2, [pc, #140]
	adds	r3, r5, r2
	ldrb	r7, [r3, #0]
	adds	r4, r5, #0
	movs	r6, #20
.L_0801e334:
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L_0801e384
	ldr	r2, [pc, #128]
	ldr	r3, [pc, #128]
	mov	sl, r2
	ldr	r2, [pc, #128]
	mov	r8, r3
	movs	r3, #255
	mov	r0, r9
	mov	lr, r2
	mov	ip, r3
.L_0801e34c:
	ldrh	r3, [r4, #0]
	mov	r1, sl
	ands	r1, r3
	adds	r3, r1, #0
	subs	r3, #128
	adds	r4, #2
	cmp	r3, #127
	bls.n	.L_0801e368
	cmp	r7, #0
	beq.n	.L_0801e37e
	cmp	r1, r8
	bls.n	.L_0801e37e
	cmp	r1, lr
	bhi.n	.L_0801e37e
.L_0801e368:
	mov	r2, ip
	ands	r1, r2
	movs	r3, #128
	eors	r1, r3
	movs	r3, #218
	lsls	r3, r3, #4
	adds	r1, r1, r3
	ldrb	r3, [r5, r1]
	movs	r2, #2
	orrs	r3, r2
	strb	r3, [r5, r1]
.L_0801e37e:
	subs	r0, #1
	cmp	r0, #0
	bne.n	.L_0801e34c
.L_0801e384:
	subs	r6, #1
	cmp	r6, #0
	bne.n	.L_0801e334
	movs	r3, #218
	lsls	r3, r3, #4
	movs	r1, #0
	movs	r6, #255
	adds	r2, r5, r3
.L_0801e394:
	ldrb	r3, [r2, #0]
	cmp	r3, #1
	bne.n	.L_0801e39c
	strb	r1, [r2, #0]
.L_0801e39c:
	subs	r6, #1
	adds	r2, #1
	cmp	r6, #0
	bge.n	.L_0801e394
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x00000ea2
	.4byte 0x000003ff
	.4byte 0x000001ff
	.4byte 0x0000027f
