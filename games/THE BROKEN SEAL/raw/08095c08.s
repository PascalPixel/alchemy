.syntax unified
	.thumb
	.set sub_08004458, 0x08004458
	.set sub_0800447c, 0x0800447c
	.set sub_08092054, 0x08092054
	.set sub_080974d8, 0x080974d8
	.set sub_0809ba34, 0x0809ba34
	.set sub_0809bb34, 0x0809bb34
	.set sub_080f9010, 0x080f9010
	.global Overlay_08095c08
Overlay_08095c08:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #436]
	movs	r1, #250
	lsls	r1, r1, #1
	adds	r3, r3, r1
	adds	r7, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl	sub_08092054
	movs	r2, #64
	adds	r2, r2, r7
	movs	r6, #0
	ldrsb	r6, [r2, r6]
	mov	r9, r0
	mov	sl, r2
	cmp	r6, #0
	bne.n	.L_08095cc2
	ldr	r2, [r7, #20]
	ldr	r3, [r7, #24]
	str	r2, [r7, #4]
	str	r3, [r7, #8]
	mov	r8, sp
	str	r2, [sp, #0]
	str	r3, [sp, #8]
	bl	sub_08004458
	adds	r5, r0, #0
	bl	sub_08004458
	lsls	r1, r5, #1
	lsls	r3, r0, #1
	adds	r3, r3, r0
	adds	r1, r1, r5
	lsls	r1, r1, #11
	lsls	r3, r3, #11
	lsrs	r3, r3, #16
	lsrs	r1, r1, #16
	subs	r1, r1, r3
	movs	r3, #192
	lsls	r3, r3, #8
	movs	r0, #240
	adds	r1, r1, r3
	lsls	r0, r0, #15
	mov	r2, r8
	bl	sub_0800447c
	mov	r1, r8
	ldr	r3, [r1, #0]
	str	r3, [r7, #12]
	ldr	r3, [r1, #8]
	str	r3, [r7, #16]
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r7, #36]
	str	r3, [r7, #32]
	adds	r3, r7, #0
	adds	r3, #66
	strb	r6, [r3, #0]
	mov	r2, sl
	ldrb	r3, [r2, #0]
	mov	r1, r9
	adds	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [r1, #80]
	ldr	r0, [r7, #0]
	ldrb	r3, [r3, #9]
	movs	r2, #12
	ands	r2, r3
	ldrb	r1, [r0, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r0, #9]
	adds	r3, r7, #0
	adds	r3, #71
	strb	r6, [r3, #0]
	strh	r6, [r7, #56]
	ldr	r3, [pc, #284]
	ldr	r3, [r3, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08095db6
	movs	r0, #134
	bl	sub_080f9010
	b.n	.L_08095db6
.L_08095cc2:
	cmp	r6, #1
	bne.n	.L_08095ce4
	movs	r2, #56
	ldrsh	r3, [r7, r2]
	cmp	r3, #3
	bne.n	.L_08095d8e
	ldr	r1, [r7, #0]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	adds	r2, r7, #0
	strb	r3, [r1, #9]
	adds	r2, #71
	movs	r3, #4
	strb	r3, [r2, #0]
	b.n	.L_08095d8e
.L_08095ce4:
	cmp	r6, #2
	bne.n	.L_08095d22
	adds	r0, r7, #0
	bl	sub_0809ba34
	cmp	r0, #0
	bne.n	.L_08095db6
	ldr	r3, [r7, #4]
	str	r3, [r7, #20]
	ldr	r3, [r7, #8]
	ldr	r1, [r7, #0]
	str	r3, [r7, #24]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	adds	r2, r7, #0
	adds	r2, #71
	strb	r3, [r1, #9]
	movs	r3, #4
	strb	r3, [r2, #0]
	adds	r3, r7, #0
	adds	r3, #68
	strb	r0, [r3, #0]
	mov	r2, sl
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	movs	r3, #40
	strh	r3, [r7, #58]
	b.n	.L_08095db6
.L_08095d22:
	cmp	r6, #3
	bne.n	.L_08095d8a
	movs	r1, #1
	mov	r8, r1
	adds	r3, r7, #0
	adds	r3, #68
	mov	r2, r8
	strb	r2, [r3, #0]
	ldr	r3, [r7, #20]
	str	r3, [r7, #4]
	ldr	r3, [r7, #24]
	str	r3, [r7, #8]
	mov	r1, r9
	ldr	r3, [r1, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	movs	r2, #160
	ldr	r3, [r1, #12]
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r1, #16]
	adds	r0, r5, #0
	str	r3, [r5, #8]
	bl	sub_080974d8
	bl	sub_08004458
	adds	r1, r0, #0
	movs	r0, #128
	adds	r2, r5, #0
	lsls	r0, r0, #11
	bl	sub_0800447c
	ldr	r3, [r5, #0]
	str	r3, [r7, #12]
	ldr	r3, [r5, #8]
	str	r3, [r7, #16]
	mov	r1, sl
	ldrb	r3, [r1, #0]
	adds	r3, #1
	strb	r3, [r1, #0]
	ldr	r3, [pc, #84]
	ldr	r3, [r3, #0]
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08095db6
	movs	r0, #145
	bl	sub_080f9010
	b.n	.L_08095db6
.L_08095d8a:
	cmp	r6, #4
	bne.n	.L_08095da2
.L_08095d8e:
	adds	r0, r7, #0
	bl	sub_0809ba34
	cmp	r0, #0
	bne.n	.L_08095db6
	mov	r1, sl
	ldrb	r3, [r1, #0]
	subs	r3, #1
	strb	r3, [r1, #0]
	b.n	.L_08095db6
.L_08095da2:
	cmp	r6, #5
	bne.n	.L_08095db6
	adds	r0, r7, #0
	bl	sub_0809ba34
	cmp	r0, #0
	bne.n	.L_08095db6
	adds	r0, r7, #0
	bl	sub_0809bb34
.L_08095db6:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x03001800
