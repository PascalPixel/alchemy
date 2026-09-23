.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_080142d4, 0x080142d4
	.set sub_0801489c, 0x0801489c
	.set sub_08014cc0, 0x08014cc0
	.set sub_08020070, 0x08020070
	.set sub_08020090, 0x08020090
	.set sub_080202c0, 0x080202c0
	.set sub_08038248, 0x08038248
	.set sub_080d2d84, 0x080d2d84
	.global Func_080d3940
	.thumb_func
Func_080d3940:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #12
	adds	r7, r1, #0
	bl	sub_080d2d84
	mov	r8, r0
	ldr	r6, [r0, #80]
	cmp	r0, #0
	beq.n	.L_080d3a04
	cmp	r6, #0
	beq.n	.L_080d3a04
	movs	r1, #0
	bl	sub_08020090
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r7, r7, r2
	mov	r2, r8
	ldr	r1, [r2, #8]
	mov	r5, sp
	str	r1, [r5, #0]
	movs	r3, #192
	ldr	r2, [r2, #16]
	lsls	r3, r3, #8
	str	r2, [r5, #8]
	ands	r7, r3
	mov	r9, r3
	movs	r3, #34
	add	r3, r8
	ldrb	r0, [r3, #0]
	mov	sl, r3
	movs	r3, #255
	bl	sub_080202c0
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r1, r7, #0
	adds	r2, r5, #0
	bl	sub_0801489c
	mov	r2, sl
	ldrb	r0, [r2, #0]
	movs	r3, #255
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #8]
	bl	sub_080202c0
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r7, r3
	beq.n	.L_080d39e0
	cmp	r7, r3
	bhi.n	.L_080d39b8
	cmp	r7, #0
	beq.n	.L_080d39c6
	b.n	.L_080d39e6
.L_080d39b8:
	movs	r3, #128
	lsls	r3, r3, #8
	cmp	r7, r3
	beq.n	.L_080d39ce
	cmp	r7, r9
	beq.n	.L_080d39da
	b.n	.L_080d39e6
.L_080d39c6:
	ldrb	r3, [r6, #22]
	subs	r3, #3
	strb	r3, [r6, #22]
	b.n	.L_080d39e0
.L_080d39ce:
	ldrb	r3, [r6, #22]
	adds	r3, #2
	strb	r3, [r6, #22]
	ldrb	r3, [r6, #23]
	adds	r3, #4
	b.n	.L_080d39e4
.L_080d39da:
	ldrb	r3, [r6, #23]
	adds	r3, #1
	b.n	.L_080d39e4
.L_080d39e0:
	ldrb	r3, [r6, #23]
	adds	r3, #5
.L_080d39e4:
	strb	r3, [r6, #23]
.L_080d39e6:
	mov	r3, r8
	adds	r3, #89
	movs	r0, #0
	strb	r0, [r3, #0]
	mov	r1, r8
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	movs	r2, #128
	lsls	r2, r2, #7
	strb	r3, [r1, #0]
	adds	r3, r7, r2
	strb	r0, [r6, #26]
	strh	r3, [r6, #18]
.L_080d3a04:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #12
	mov	r8, r1
	adds	r5, r2, #0
	bl	sub_080d2d84
	adds	r6, r0, #0
	ldr	r7, [r6, #80]
	cmp	r6, #0
	beq.n	.L_080d3b16
	cmp	r7, #0
	beq.n	.L_080d3b16
	movs	r2, #1
	negs	r2, r2
	cmp	r5, r2
	beq.n	.L_080d3a3e
	adds	r1, r5, #0
	bl	sub_08020090
.L_080d3a3e:
	ldr	r1, [r6, #8]
	mov	r5, sp
	movs	r3, #128
	str	r1, [r5, #0]
	lsls	r3, r3, #6
	add	r8, r3
	movs	r2, #192
	lsls	r2, r2, #8
	mov	r3, r8
	ands	r3, r2
	mov	r9, r2
	ldr	r2, [r6, #16]
	mov	r8, r3
	str	r2, [r5, #8]
	movs	r3, #34
	adds	r3, r3, r6
	ldrb	r0, [r3, #0]
	mov	sl, r3
	movs	r3, #255
	bl	sub_080202c0
	movs	r0, #128
	lsls	r0, r0, #13
	mov	r1, r8
	adds	r2, r5, #0
	bl	sub_0801489c
	mov	r2, sl
	ldrb	r0, [r2, #0]
	movs	r3, #255
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #8]
	bl	sub_080202c0
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r8, r3
	beq.n	.L_080d3ae8
	cmp	r8, r3
	bhi.n	.L_080d3a96
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_080d3aa4
	b.n	.L_080d3af8
.L_080d3a96:
	movs	r2, #128
	lsls	r2, r2, #8
	cmp	r8, r2
	beq.n	.L_080d3ac4
	cmp	r8, r9
	beq.n	.L_080d3ae2
	b.n	.L_080d3af8
.L_080d3aa4:
	ldr	r3, [r6, #8]
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r6, #8]
	ldr	r3, [r6, #16]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r6, #16]
	ldrb	r3, [r7, #22]
	subs	r3, #4
	strb	r3, [r7, #22]
	ldrb	r3, [r7, #23]
	adds	r3, #15
	b.n	.L_080d3af6
.L_080d3ac4:
	ldr	r3, [r6, #8]
	ldr	r2, [pc, #92]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	ldr	r3, [r6, #16]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r6, #16]
	ldrb	r3, [r7, #22]
	adds	r3, #3
	strb	r3, [r7, #22]
	ldrb	r3, [r7, #23]
	adds	r3, #16
	b.n	.L_080d3af6
.L_080d3ae2:
	ldrb	r3, [r7, #23]
	adds	r3, #1
	b.n	.L_080d3af6
.L_080d3ae8:
	ldr	r3, [r6, #16]
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r6, #16]
	ldrb	r3, [r7, #23]
	adds	r3, #13
.L_080d3af6:
	strb	r3, [r7, #23]
.L_080d3af8:
	adds	r3, r6, #0
	adds	r3, #89
	movs	r0, #0
	strb	r0, [r3, #0]
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	add	r3, r8
	strb	r0, [r7, #26]
	strh	r3, [r7, #18]
.L_080d3b16:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #4
	adds	r6, r1, #0
	bl	sub_080d2d84
	mov	r8, r0
	cmp	r0, #0
	beq.n	.L_080d3bdc
	mov	r3, r8
	adds	r3, #84
	ldrb	r3, [r3, #0]
	mov	r9, r3
	cmp	r3, #1
	bne.n	.L_080d3bdc
	movs	r3, #128
	lsls	r3, r3, #8
	mov	r2, r8
	ands	r3, r6
	ldr	r7, [r2, #80]
	cmp	r3, #0
	beq.n	.L_080d3b5c
	movs	r6, #0
.L_080d3b5c:
	movs	r1, #193
	lsls	r1, r1, #3
	movs	r0, #68
	bl	sub_08014cc0
	movs	r2, #128
	movs	r3, #0
	adds	r5, r0, #0
	lsls	r2, r2, #3
	mov	r0, sp
	adds	r5, r5, r2
	mov	sl, r3
	str	r3, [r0, #0]
	movs	r2, #133
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, r5, #0
	adds	r2, #32
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r6, #0
	bl	sub_08038248
	adds	r2, r5, #0
	movs	r1, #128
	ldrb	r0, [r7, #16]
	bl	sub_080142d4
	adds	r5, r0, #0
	movs	r0, #68
	bl	sub_0801314c
	mov	r3, r8
	adds	r3, #92
	mov	r2, r9
	strb	r2, [r3, #0]
	ldr	r0, [r7, #40]
	bl	sub_08020070
	mov	r3, sl
	ldrb	r2, [r7, #5]
	str	r3, [r7, #40]
	strb	r3, [r7, #27]
	movs	r3, #33
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r7, #5]
	ldr	r3, [pc, #20]
	ldrh	r2, [r7, #8]
	ands	r5, r3
	ldr	r3, [pc, #16]
	ands	r3, r2
	orrs	r3, r5
	mov	r2, sl
	strh	r3, [r7, #8]
	strb	r2, [r7, #25]
	strb	r2, [r7, #26]
	b.n	.L_080d3bdc
	.4byte 0x000003ff
	.2byte 0xfc00
	.2byte 0xffff
.L_080d3bdc:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r3, r3, r2
	strh	r0, [r3, #0]
	bx	lr
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #255
	ands	r3, r0
	lsls	r3, r3, #2
	adds	r3, #20
	ldr	r2, [r2, r3]
	movs	r1, #1
	negs	r1, r1
	cmp	r2, #0
	beq.n	.L_080d3c28
	adds	r3, r2, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080d3c28
	ldr	r3, [r2, #80]
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrsh	r1, [r3, r2]
.L_080d3c28:
	adds	r0, r1, #0
	pop	{pc}
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #108]
	movs	r5, #1
	ldr	r2, [r4, #52]
	negs	r5, r5
	movs	r1, #8
	cmp	r2, #0
	beq.n	.L_080d3c5a
	adds	r3, r2, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080d3c5a
	ldr	r3, [r2, #80]
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, r0
	bne.n	.L_080d3c5a
	movs	r5, #8
	b.n	.L_080d3c82
.L_080d3c5a:
	adds	r1, #1
	cmp	r1, #79
	bgt.n	.L_080d3c82
	lsls	r3, r1, #2
	adds	r3, #20
	ldr	r2, [r4, r3]
	cmp	r2, #0
	beq.n	.L_080d3c5a
	adds	r3, r2, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080d3c5a
	ldr	r3, [r2, #80]
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, r0
	bne.n	.L_080d3c5a
	adds	r5, r1, #0
.L_080d3c82:
	adds	r0, r5, #0
	pop	{r5, pc}
	.2byte 0x0000
