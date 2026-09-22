.syntax unified
	.thumb
	.set sub_08009080, 0x08009080
	.set sub_08009098, 0x08009098
	.set sub_080090c8, 0x080090c8
	.set sub_080090d0, 0x080090d0
	.set sub_0808ba1c, 0x0808ba1c
	.global Overlay_08093874
Overlay_08093874:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r0
	mov	r8, r1
	bl	sub_0808ba1c
	adds	r7, r0, #0
	movs	r5, #0
	movs	r6, #0
	cmp	r7, #0
	beq.n	.L_08093958
	movs	r3, #3
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080938b2
	cmp	r3, #2
	beq.n	.L_080938a2
	ldr	r3, [r7, #104]
	cmp	r3, #0
	bne.n	.L_080938c2
.L_080938a2:
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	movs	r0, #209
	bl	sub_080090c8
	adds	r5, r0, #0
	b.n	.L_080938c2
.L_080938b2:
	ldr	r5, [r7, #104]
	cmp	r5, #0
	beq.n	.L_08093958
	adds	r0, r5, #0
	bl	sub_080090d0
	str	r6, [r7, #104]
	b.n	.L_08093958
.L_080938c2:
	cmp	r5, #0
	beq.n	.L_08093958
	movs	r6, #3
	mov	r2, r8
	ands	r6, r2
	cmp	r6, #1
	beq.n	.L_080938d6
	cmp	r6, #2
	beq.n	.L_080938e8
	b.n	.L_08093900
.L_080938d6:
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08009080
	adds	r3, r5, #0
	adds	r3, #100
	str	r5, [r7, #104]
	strh	r6, [r3, #0]
	b.n	.L_08093900
.L_080938e8:
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_08009080
	ldr	r1, [pc, #72]
	adds	r0, r5, #0
	bl	sub_08009098
	adds	r2, r5, #0
	adds	r2, #100
	movs	r3, #1
	strh	r3, [r2, #0]
.L_08093900:
	adds	r3, r5, #0
	ldr	r2, [pc, #52]
	adds	r3, #102
	mov	r1, sl
	strh	r1, [r3, #0]
	subs	r3, #17
	strb	r2, [r3, #0]
	ldr	r3, [pc, #48]
	ldr	r6, [r5, #80]
	str	r3, [r5, #108]
	adds	r3, r6, #0
	adds	r3, #38
	strb	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r2, r8
	ands	r3, r2
	str	r7, [r5, #104]
	cmp	r3, #0
	beq.n	.L_08093944
	ldrb	r3, [r6, #9]
	movs	r2, #13
	negs	r2, r2
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	strb	r2, [r6, #9]
	b.n	.L_08093958
	.4byte 0x00000000
	.4byte 0x0809fd38
	.2byte 0x376d
	.2byte 0x0809
.L_08093944:
	ldr	r3, [r7, #80]
	ldrb	r3, [r3, #9]
	movs	r2, #12
	ands	r2, r3
	ldrb	r1, [r6, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r6, #9]
.L_08093958:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
