.syntax unified
	.thumb
	.set sub_08015120, 0x08015120
	.set sub_080151c8, 0x080151c8
	.set sub_08077008, 0x08077008
	.set sub_08077080, 0x08077080
	.global Overlay_080be18c
Overlay_080be18c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	add	r3, sp, #24
	mov	r1, r9
	str	r1, [r3, #0]
	mov	sl, r1
	bl	sub_08077080
	movs	r3, #0
	ldrb	r2, [r0, #0]
	str	r3, [sp, #20]
	ldrb	r3, [r0, #3]
	cmp	r3, #5
	beq.n	.L_080be1c0
	cmp	r3, #5
	blt.n	.L_080be1c4
	cmp	r3, #57
	bgt.n	.L_080be1c4
	cmp	r3, #56
	blt.n	.L_080be1c4
.L_080be1c0:
	movs	r0, #1
	str	r0, [sp, #20]
.L_080be1c4:
	cmp	r2, #0
	beq.n	.L_080be1dc
	cmp	r2, #4
	beq.n	.L_080be1f4
	movs	r2, #12
	negs	r2, r2
	movs	r1, #0
	add	r2, sl
	mov	r9, r1
	movs	r6, #0
	mov	fp, r2
	b.n	.L_080be210
.L_080be1dc:
	mov	r3, sl
	subs	r3, #4
	ldr	r3, [r3, #0]
	movs	r1, #1
	strb	r2, [r3, #16]
	strb	r1, [r3, #1]
	mov	r2, sl
	subs	r2, #8
	ldr	r2, [r2, #0]
	strb	r1, [r3, #30]
	strb	r2, [r3, #2]
	b.n	.L_080be35c
.L_080be1f4:
	mov	r3, sl
	subs	r3, #4
	ldr	r2, [r3, #0]
	movs	r1, #1
	movs	r3, #0
	strb	r3, [r2, #16]
	strb	r1, [r2, #1]
	mov	r3, sl
	subs	r3, #8
	ldr	r3, [r3, #0]
	strb	r1, [r2, #30]
	strb	r3, [r2, #2]
	b.n	.L_080be35c
.L_080be20e:
	adds	r6, #1
.L_080be210:
	mov	r3, fp
	ldr	r1, [r3, #0]
	lsls	r3, r6, #1
	adds	r3, #88
	ldrsh	r3, [r1, r3]
	cmp	r3, #255
	bne.n	.L_080be20e
	str	r6, [sp, #16]
	movs	r3, #100
	adds	r2, r1, #2
	ldrsh	r3, [r2, r3]
	movs	r6, #0
	cmp	r3, #255
	beq.n	.L_080be23a
	adds	r2, #100
.L_080be22e:
	adds	r2, #2
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	adds	r6, #1
	cmp	r3, #255
	bne.n	.L_080be22e
.L_080be23a:
	mov	r2, sl
	subs	r2, #16
	str	r6, [sp, #12]
	str	r2, [sp, #8]
	ldr	r2, [r2, #0]
	ldrh	r3, [r2, #10]
	movs	r4, #15
	ands	r4, r3
	movs	r0, #12
	ldrsh	r3, [r2, r0]
	subs	r2, r4, r3
	adds	r3, r4, r3
	subs	r3, #1
	adds	r6, r2, #1
	str	r3, [sp, #4]
	cmp	r6, r3
	bgt.n	.L_080be318
	movs	r1, #4
	negs	r1, r1
	lsls	r3, r6, #1
	add	r1, sl
	adds	r7, r3, #0
	mov	r8, r1
	adds	r7, #100
.L_080be26a:
	cmp	r6, #0
	blt.n	.L_080be30c
	ldr	r2, [sp, #8]
	ldr	r3, [r2, #0]
	ldrh	r2, [r3, #10]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080be2c4
	ldr	r3, [sp, #12]
	cmp	r6, r3
	bge.n	.L_080be30c
	mov	r0, fp
	ldr	r3, [r0, #0]
	adds	r3, #2
	ldrsh	r5, [r3, r7]
	cmp	r5, #254
	beq.n	.L_080be30c
	ldr	r2, [sp, #20]
	cmp	r2, #0
	bne.n	.L_080be2a6
	adds	r0, r5, #0
	str	r4, [sp, #0]
	bl	sub_08077008
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	ldr	r4, [sp, #0]
	cmp	r3, #0
	beq.n	.L_080be30c
.L_080be2a6:
	mov	r2, r8
	ldr	r0, [r2, #0]
	mov	r2, r9
	adds	r1, r0, #2
	adds	r2, #28
	movs	r3, #1
	strb	r3, [r1, r2]
	subs	r3, r6, r4
	subs	r2, #12
	strb	r3, [r0, r2]
	mov	r3, r9
	movs	r0, #1
	strb	r5, [r1, r3]
	add	r9, r0
	b.n	.L_080be30c
.L_080be2c4:
	ldr	r1, [sp, #16]
	cmp	r6, r1
	bge.n	.L_080be30c
	mov	r3, fp
	ldr	r2, [r3, #0]
	lsls	r3, r6, #1
	adds	r3, #88
	ldrsh	r5, [r2, r3]
	cmp	r5, #254
	beq.n	.L_080be30c
	ldr	r1, [sp, #20]
	cmp	r1, #0
	bne.n	.L_080be2f0
	adds	r0, r5, #0
	str	r4, [sp, #0]
	bl	sub_08077008
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	ldr	r4, [sp, #0]
	cmp	r3, #0
	beq.n	.L_080be30c
.L_080be2f0:
	mov	r3, r8
	ldr	r0, [r3, #0]
	mov	r2, r9
	adds	r1, r0, #2
	adds	r2, #28
	movs	r3, #1
	strb	r3, [r1, r2]
	subs	r2, #12
	subs	r3, r6, r4
	strb	r3, [r0, r2]
	mov	r0, r9
	strb	r5, [r1, r0]
	movs	r1, #1
	add	r9, r1
.L_080be30c:
	ldr	r2, [sp, #4]
	adds	r6, #1
	adds	r7, #2
	cmp	r6, r2
	ble.n	.L_080be26a
	b.n	.L_080be320
.L_080be318:
	movs	r3, #4
	negs	r3, r3
	add	r3, sl
	mov	r8, r3
.L_080be320:
	mov	r0, r8
	ldr	r3, [r0, #0]
	mov	r1, r9
	mov	r2, r9
	strb	r1, [r3, #1]
	cmp	r2, #0
	bgt.n	.L_080be35c
	ldr	r0, [sp, #8]
	ldr	r3, [r0, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #48]
	bl	sub_080151c8
	mov	r3, sl
	subs	r3, #20
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #40]
	adds	r2, r3, r0
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	bne.n	.L_080be358
	movs	r3, #1
	strb	r3, [r2, #0]
.L_080be358:
	movs	r0, #1
	negs	r0, r0
.L_080be35c:
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000816
	.4byte 0x0000012b
