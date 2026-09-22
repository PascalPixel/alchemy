.syntax unified
	.thumb
	.set sub_08103064, 0x08103064
	.global Overlay_08101860
Overlay_08101860:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	mov	r8, r1
	movs	r1, #0
	str	r2, [sp, #8]
	str	r3, [sp, #4]
	str	r1, [sp, #0]
	mov	r2, r8
	ldrh	r3, [r2, #0]
	mov	ip, r0
	mov	sl, r1
	mov	fp, r1
	cmp	r3, #0
	beq.n	.L_081018f8
	movs	r3, #252
	ldr	r5, [sp, #8]
	lsls	r3, r3, #6
	adds	r3, #255
	mov	lr, r3
	mov	r0, r8
	subs	r5, #2
.L_08101896:
	ldrh	r2, [r0, #0]
	mov	r3, lr
	ands	r3, r2
	strh	r3, [r5, #2]
	movs	r1, #1
	add	sl, r1
	mov	r1, ip
	ldrh	r2, [r1, #0]
	ldrh	r3, [r0, #0]
	adds	r5, #2
	eors	r3, r2
	mov	r2, lr
	ands	r3, r2
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_081018ce
	ldr	r7, [pc, #40]
	adds	r6, r0, #0
.L_081018ba:
	adds	r4, #1
	cmp	r4, #31
	bgt.n	.L_081018ce
	adds	r1, #4
	ldrh	r3, [r6, #0]
	ldrh	r2, [r1, #0]
	eors	r3, r2
	ands	r3, r7
	cmp	r3, #0
	bne.n	.L_081018ba
.L_081018ce:
	cmp	r4, #32
	bne.n	.L_081018e8
	movs	r3, #1
	add	fp, r3
	ldr	r2, [pc, #12]
	ldrh	r3, [r5, #0]
	orrs	r3, r2
	strh	r3, [r5, #0]
	b.n	.L_081018e8
	.4byte 0x00003fff
	.2byte 0x8000
	.2byte 0x0000
.L_081018e8:
	mov	r3, r8
	adds	r0, #4
	adds	r3, #124
	cmp	r0, r3
	bgt.n	.L_081018f8
	ldrh	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_08101896
.L_081018f8:
	mov	r2, ip
	ldrh	r3, [r2, #0]
	movs	r1, #0
	mov	r9, r1
	cmp	r3, #0
	beq.n	.L_08101988
	ldr	r2, [sp, #8]
	mov	r1, sl
	lsls	r3, r1, #1
	mov	lr, ip
	adds	r0, r3, r2
	movs	r7, #0
.L_08101910:
	mov	r1, ip
	ldrh	r3, [r7, r1]
	mov	r1, r8
	ldrh	r2, [r1, #0]
	movs	r4, #0
	eors	r3, r2
	movs	r2, #252
	lsls	r2, r2, #6
	adds	r2, #255
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08101948
	ldr	r6, [pc, #24]
	mov	r5, lr
.L_0810192c:
	adds	r4, #1
	cmp	r4, #31
	bgt.n	.L_08101948
	adds	r1, #4
	ldrh	r3, [r5, #0]
	ldrh	r2, [r1, #0]
	eors	r3, r2
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_0810192c
	b.n	.L_08101948
	movs	r0, r0
	.2byte 0x3fff
	.2byte 0x0000
.L_08101948:
	cmp	r4, #32
	bne.n	.L_0810196a
	ldr	r3, [sp, #0]
	mov	r1, ip
	adds	r3, #1
	str	r3, [sp, #0]
	ldrh	r3, [r7, r1]
	movs	r2, #252
	lsls	r2, r2, #6
	adds	r2, #255
	ands	r2, r3
	ldr	r3, [pc, #36]
	orrs	r2, r3
	strh	r2, [r0, #0]
	movs	r2, #1
	adds	r0, #2
	add	sl, r2
.L_0810196a:
	movs	r1, #1
	add	r9, r1
	movs	r3, #4
	mov	r2, r9
	adds	r7, #4
	add	lr, r3
	cmp	r2, #31
	bgt.n	.L_08101988
	mov	r1, ip
	ldrh	r3, [r7, r1]
	cmp	r3, #0
	bne.n	.L_08101910
	b.n	.L_08101988
	.2byte 0x4000
	.2byte 0x0000
.L_08101988:
	ldr	r3, [sp, #4]
	mov	r2, fp
	str	r2, [r3, #0]
	ldr	r1, [sp, #0]
	ldr	r3, [sp, #44]
	mov	r0, sl
	str	r1, [r3, #0]
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r4, #0
	mov	r8, r3
	movs	r3, #139
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, r8
	ldrb	r3, [r3, #0]
	sub	sp, #4
	cmp	r4, r3
	bge.n	.L_081019fa
	movs	r5, #129
	adds	r7, r0, #0
	lsls	r5, r5, #2
	adds	r7, #160
	adds	r6, r0, #0
	add	r5, r8
.L_081019d2:
	movs	r2, #1
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	negs	r2, r2
	str	r4, [sp, #0]
	bl	sub_08103064
	movs	r3, #139
	lsls	r3, r3, #1
	adds	r3, #255
	ldr	r4, [sp, #0]
	add	r3, r8
	ldrb	r3, [r3, #0]
	adds	r4, #1
	strb	r0, [r7, #0]
	adds	r5, #2
	adds	r7, #1
	adds	r6, #20
	cmp	r4, r3
	blt.n	.L_081019d2
.L_081019fa:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
