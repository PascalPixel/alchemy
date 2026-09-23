.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08016ca4, 0x08016ca4
	.set sub_08020030, 0x08020030
	.set sub_08020100, 0x08020100
	.set sub_080ad148, 0x080ad148
	.set sub_0811bc64, 0x0811bc64
	.set sub_0811bdb0, 0x0811bdb0
	.set sub_0811be3c, 0x0811be3c
	.set sub_0811f3b8, 0x0811f3b8
	.global Func_0811f444
	.thumb_func
Func_0811f444:
	push	{r5, r6, r7, lr}
	sub	sp, #16
	adds	r6, r0, #0
	bl	sub_08016ca4
	movs	r5, #0
	adds	r7, r0, #0
	b.n	.L_0811f470
.L_0811f454:
	movs	r2, #149
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L_0811f468
	movs	r1, #4
	bl	sub_08020030
	b.n	.L_0811f46e
.L_0811f468:
	movs	r1, #5
	bl	sub_08020030
.L_0811f46e:
	adds	r5, #1
.L_0811f470:
	adds	r0, r6, #0
	bl	sub_0811be3c
	adds	r1, r5, #0
	ldr	r0, [r0, #0]
	bl	sub_0811bdb0
	cmp	r0, #0
	bne.n	.L_0811f454
	movs	r2, #149
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_0811f4d0
	movs	r5, #0
	mov	r7, sp
	b.n	.L_0811f4a4
.L_0811f494:
	ldr	r2, [r0, #40]
	lsls	r3, r5, #2
	str	r0, [r7, r3]
	movs	r3, #6
	strb	r3, [r2, #5]
	movs	r3, #255
	strb	r3, [r2, #22]
	adds	r5, #1
.L_0811f4a4:
	adds	r0, r6, #0
	bl	sub_0811be3c
	adds	r1, r5, #0
	ldr	r0, [r0, #0]
	bl	sub_0811bdb0
	cmp	r0, #0
	bne.n	.L_0811f494
	movs	r0, #4
	bl	sub_08013560
	adds	r0, r6, #0
	bl	sub_0811f3b8
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_08020100
	adds	r0, r6, #0
	bl	sub_0811bc64
.L_0811f4d0:
	add	sp, #16
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	movs	r1, #0
	lsls	r3, r3, #18
	sub	sp, #12
	mov	r8, r1
	ldr	r2, [r3, #36]
	cmp	r0, #0
	beq.n	.L_0811f530
	movs	r3, #88
	ldrsh	r3, [r2, r3]
	movs	r7, #0
	cmp	r3, #255
	beq.n	.L_0811f580
	adds	r5, r2, #0
	adds	r5, #88
	mov	r6, sp
.L_0811f4fc:
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	cmp	r0, #254
	beq.n	.L_0811f51e
	bl	sub_08016ca4
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	beq.n	.L_0811f51e
	ldr	r2, [pc, #24]
	adds	r3, r7, #0
	orrs	r3, r2
	movs	r2, #1
	strh	r3, [r6, #0]
	add	r8, r2
	adds	r6, #2
.L_0811f51e:
	adds	r5, #2
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	adds	r7, #1
	cmp	r3, #255
	bne.n	.L_0811f4fc
	b.n	.L_0811f580
	.2byte 0x0100
	.2byte 0x0000
.L_0811f530:
	adds	r2, #2
	movs	r3, #100
	ldrsh	r3, [r2, r3]
	movs	r7, #0
	mov	sl, r2
	cmp	r3, #255
	beq.n	.L_0811f580
	mov	r1, r8
	lsls	r3, r1, #1
	add	r1, sp, #12
	adds	r3, r3, r1
	adds	r5, r3, #0
	movs	r6, #100
	subs	r5, #12
.L_0811f54c:
	ldrsh	r0, [r2, r6]
	cmp	r0, #254
	beq.n	.L_0811f56c
	bl	sub_08016ca4
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	beq.n	.L_0811f56c
	ldr	r2, [pc, #28]
	adds	r3, r7, #0
	orrs	r3, r2
	movs	r2, #1
	strh	r3, [r5, #0]
	add	r8, r2
	adds	r5, #2
.L_0811f56c:
	adds	r6, #2
	mov	r2, sl
	ldrsh	r3, [r2, r6]
	adds	r7, #1
	cmp	r3, #255
	bne.n	.L_0811f54c
	b.n	.L_0811f580
	movs	r0, r0
	.2byte 0x0180
	.2byte 0x0000
.L_0811f580:
	mov	r2, r8
	movs	r0, #0
	cmp	r2, #0
	beq.n	.L_0811f598
	mov	r5, sp
	bl	sub_080ad148
	mov	r3, r8
	muls	r3, r0
	lsrs	r3, r3, #16
	lsls	r3, r3, #1
	ldrh	r0, [r5, r3]
.L_0811f598:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
