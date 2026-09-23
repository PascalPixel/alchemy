.syntax unified
	.thumb
	.global Func_0802d400
	.thumb_func
Func_0802d400:
	ldr	r3, [pc, #12]
	lsls	r2, r2, #4
	subs	r2, r2, r1
	adds	r2, #15
	ldrb	r3, [r3, r2]
	ldrsb	r0, [r0, r3]
	lsls	r0, r0, #19
	bx	lr
	.2byte 0xeec4
	.2byte 0x0802
	push	{lr}
	cmp	r2, #7
	bhi.n	.L_0802d42e
	cmp	r1, #7
	bhi.n	.L_0802d426
	ldrb	r0, [r0, #0]
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	b.n	.L_0802d434
.L_0802d426:
	ldrb	r0, [r0, #1]
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	b.n	.L_0802d434
.L_0802d42e:
	ldrb	r0, [r0, #2]
	lsls	r0, r0, #24
	asrs	r0, r0, #24
.L_0802d434:
	lsls	r0, r0, #19
	pop	{pc}
	push	{lr}
	cmp	r2, #7
	bhi.n	.L_0802d446
	ldrb	r0, [r0, #0]
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	b.n	.L_0802d458
.L_0802d446:
	cmp	r1, #7
	bhi.n	.L_0802d452
	ldrb	r0, [r0, #1]
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	b.n	.L_0802d458
.L_0802d452:
	ldrb	r0, [r0, #2]
	lsls	r0, r0, #24
	asrs	r0, r0, #24
.L_0802d458:
	lsls	r0, r0, #19
	pop	{pc}
.L_0802d45c:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r5, r1, #0
	ldr	r1, [r3, #32]
	adds	r6, r2, #0
	asrs	r5, r5, #16
	asrs	r6, r6, #16
	cmp	r1, #0
	beq.n	.L_0802d484
	movs	r2, #3
	ands	r2, r0
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #158
	lsls	r3, r3, #3
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r1, [r1, r3]
	b.n	.L_0802d486
.L_0802d484:
	ldr	r1, [pc, #64]
.L_0802d486:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_0802d48e
	adds	r3, #15
.L_0802d48e:
	asrs	r2, r3, #4
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_0802d498
	adds	r3, #15
.L_0802d498:
	asrs	r3, r3, #4
	lsls	r3, r3, #7
	adds	r3, r2, r3
	ldrb	r1, [r1, r3]
	ldr	r3, [pc, #40]
	lsls	r1, r1, #2
	adds	r0, r1, r3
	ldrb	r0, [r0, #0]
	movs	r2, #15
	ldr	r4, [pc, #36]
	adds	r3, r2, #0
	ldr	r7, [pc, #36]
	ands	r3, r0
	ands	r5, r2
	ands	r6, r2
	lsls	r3, r3, #2
	adds	r0, r1, r7
	ldr	r3, [r4, r3]
	adds	r1, r5, #0
	adds	r2, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbde0
	movs	r0, r0
	.4byte 0x02024000
	.4byte 0x0202c000
	.4byte 0x0802efc4
	.2byte 0xc001
	.2byte 0x0202
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r5, r0, #0
	ldr	r0, [r3, #32]
	adds	r4, r2, #0
	asrs	r1, r1, #16
	asrs	r4, r4, #16
	cmp	r0, #0
	beq.n	.L_0802d500
	movs	r2, #3
	ands	r2, r5
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #158
	lsls	r3, r3, #3
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r0, r3]
	b.n	.L_0802d502
.L_0802d500:
	ldr	r0, [pc, #36]
.L_0802d502:
	cmp	r1, #0
	bge.n	.L_0802d508
	adds	r1, #15
.L_0802d508:
	adds	r2, r4, #0
	asrs	r1, r1, #4
	cmp	r2, #0
	bge.n	.L_0802d512
	adds	r2, #15
.L_0802d512:
	asrs	r3, r2, #4
	lsls	r3, r3, #7
	adds	r3, r1, r3
	ldrb	r3, [r0, r3]
	ldr	r2, [pc, #16]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	movs	r0, #15
	ands	r0, r3
	pop	{r5, pc}
	.4byte 0x02024000
	.2byte 0xc000
	.2byte 0x0202
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r5, r0, #0
	ldr	r0, [r3, #32]
	adds	r4, r2, #0
	asrs	r1, r1, #16
	asrs	r4, r4, #16
	cmp	r0, #0
	beq.n	.L_0802d558
	movs	r2, #3
	ands	r2, r5
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #158
	lsls	r3, r3, #3
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r0, r3]
	b.n	.L_0802d55a
.L_0802d558:
	ldr	r0, [pc, #48]
.L_0802d55a:
	cmp	r1, #0
	bge.n	.L_0802d560
	adds	r1, #15
.L_0802d560:
	adds	r2, r4, #0
	asrs	r1, r1, #4
	cmp	r2, #0
	bge.n	.L_0802d56a
	adds	r2, #15
.L_0802d56a:
	asrs	r3, r2, #4
	lsls	r3, r3, #7
	adds	r3, r1, r3
	ldrb	r3, [r0, r3]
	ldr	r2, [pc, #28]
	lsls	r3, r3, #2
	adds	r1, r3, r2
	ldrb	r0, [r1, #0]
	cmp	r0, #7
	bne.n	.L_0802d588
	ldrb	r2, [r1, #1]
	ldrb	r3, [r1, #2]
	cmp	r2, r3
	beq.n	.L_0802d588
	movs	r0, #99
.L_0802d588:
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x02024000
	.2byte 0xc000
	.2byte 0x0202
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	r8, r1
	adds	r6, r2, #0
	adds	r7, r0, #0
	mov	sl, r3
	bl	.L_0802d45c
	mov	r2, r8
	mov	r3, sl
	adds	r5, r0, #0
	subs	r1, r2, r3
	adds	r0, r7, #0
	adds	r2, r6, #0
	bl	.L_0802d45c
	cmp	r5, r0
	bge.n	.L_0802d5be
	adds	r5, r0, #0
.L_0802d5be:
	mov	r1, r8
	add	r1, sl
	adds	r0, r7, #0
	adds	r2, r6, #0
	bl	.L_0802d45c
	cmp	r5, r0
	bge.n	.L_0802d5d0
	adds	r5, r0, #0
.L_0802d5d0:
	mov	r3, sl
	subs	r2, r6, r3
	adds	r0, r7, #0
	mov	r1, r8
	bl	.L_0802d45c
	cmp	r5, r0
	bge.n	.L_0802d5e2
	adds	r5, r0, #0
.L_0802d5e2:
	mov	r3, sl
	adds	r2, r6, r3
	adds	r0, r7, #0
	mov	r1, r8
	bl	.L_0802d45c
	cmp	r5, r0
	bge.n	.L_0802d5f4
	adds	r5, r0, #0
.L_0802d5f4:
	adds	r0, r5, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
