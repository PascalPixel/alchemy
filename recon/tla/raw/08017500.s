.syntax unified
	.thumb
	.set sub_08017054, 0x08017054
	.set sub_080172e4, 0x080172e4
	.set sub_08017380, 0x08017380
	.set sub_0801787c, 0x0801787c
	.set sub_08017884, 0x08017884
	.global Func_08017500
	.thumb_func
Func_08017500:
	push	{lr}
	bl	sub_0801787c
	cmp	r0, #0
	bne.n	.L_0801750e
	movs	r0, #0
	b.n	.L_08017510
.L_0801750e:
	ldr	r0, [pc, #4]
.L_08017510:
	pop	{r1}
	bx	r1
	.2byte 0x8004
	.2byte 0x0000
	push	{r4, r5, r6, r7, lr}
	sub	sp, #64
	adds	r7, r1, #0
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #15
	bls.n	.L_08017530
	ldr	r0, [pc, #4]
	b.n	.L_080175ac
	movs	r0, r0
	.2byte 0x80ff
	.2byte 0x0000
.L_08017530:
	adds	r0, r4, #0
	bl	sub_080172e4
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	cmp	r5, #0
	bne.n	.L_080175aa
	mov	r0, sp
	bl	sub_08017054
	ldr	r3, [pc, #36]
	ldrh	r1, [r3, #0]
	ldr	r0, [pc, #36]
	ands	r1, r0
	ldr	r0, [pc, #36]
	ldr	r2, [r0, #0]
	ldrh	r0, [r2, #16]
	orrs	r0, r1
	strh	r0, [r3, #0]
	ldr	r1, [pc, #32]
	ldr	r0, [r2, #4]
	strh	r0, [r1, #0]
	ldrb	r0, [r2, #8]
	lsls	r4, r0
	movs	r0, #224
	lsls	r0, r0, #20
	adds	r4, r4, r0
	adds	r6, r1, #0
	b.n	.L_08017586
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x02007408
	.2byte 0x740c
	.2byte 0x0200
.L_0801757c:
	ldrh	r0, [r6, #0]
	subs	r0, #1
	strh	r0, [r6, #0]
	adds	r7, #1
	adds	r4, #1
.L_08017586:
	ldrh	r0, [r6, #0]
	cmp	r0, #0
	beq.n	.L_0801759c
	adds	r0, r7, #0
	adds	r1, r4, #0
	bl	sub_08017380
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	cmp	r5, #0
	beq.n	.L_0801757c
.L_0801759c:
	ldr	r2, [pc, #20]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #20]
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
.L_080175aa:
	adds	r0, r5, #0
.L_080175ac:
	add	sp, #64
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x04000204
	.2byte 0xfffc
	.2byte 0x0000
	push	{r4, r5, r6, lr}
	sub	sp, #64
	mov	r0, sp
	bl	sub_08017054
	ldr	r5, [pc, #76]
	ldrh	r0, [r5, #0]
	ldr	r6, [pc, #76]
	ands	r0, r6
	ldr	r1, [pc, #76]
	ldrh	r1, [r1, #36]
	orrs	r0, r1
	strh	r0, [r5, #0]
	ldr	r1, [pc, #72]
	movs	r4, #170
	strb	r4, [r1, #0]
	ldr	r3, [pc, #68]
	movs	r2, #85
	strb	r2, [r3, #0]
	movs	r0, #128
	strb	r0, [r1, #0]
	strb	r4, [r1, #0]
	strb	r2, [r3, #0]
	movs	r0, #16
	strb	r0, [r1, #0]
	ldr	r0, [pc, #56]
	movs	r1, #224
	lsls	r1, r1, #20
	ldr	r3, [r0, #0]
	movs	r0, #3
	movs	r2, #255
	bl	sub_08017884
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	ldrh	r1, [r5, #0]
	ands	r1, r6
	movs	r2, #3
	orrs	r1, r2
	strh	r1, [r5, #0]
	add	sp, #64
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x08017f68
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.2byte 0x7400
	.2byte 0x0200
.L_0801762c:
	push	{r4, r5, lr}
	lsls	r0, r0, #16
	ldr	r3, [pc, #104]
	lsrs	r0, r0, #16
	adds	r4, r0, #0
	ldrb	r1, [r3, #28]
	lsls	r4, r1
	movs	r0, #224
	lsls	r0, r0, #20
	adds	r4, r4, r0
	ldr	r1, [pc, #92]
	ldrh	r0, [r1, #0]
	adds	r5, r0, #0
	movs	r0, #0
	strh	r0, [r1, #0]
	ldr	r2, [pc, #88]
	movs	r0, #170
	strb	r0, [r2, #0]
	ldr	r1, [pc, #84]
	movs	r0, #85
	strb	r0, [r1, #0]
	movs	r0, #160
	strb	r0, [r2, #0]
	ldr	r0, [r3, #24]
	cmp	r0, #0
	beq.n	.L_0801766c
	movs	r1, #255
.L_08017662:
	strb	r1, [r4, #0]
	adds	r4, #1
	subs	r0, #1
	cmp	r0, #0
	bne.n	.L_08017662
.L_0801766c:
	subs	r4, #1
	ldr	r0, [pc, #48]
	strh	r5, [r0, #0]
	ldr	r0, [pc, #56]
	ldr	r3, [r0, #0]
	movs	r0, #1
	adds	r1, r4, #0
	movs	r2, #255
	bl	sub_08017884
	lsls	r0, r0, #16
	lsrs	r1, r0, #16
	cmp	r1, #0
	beq.n	.L_08017692
	movs	r0, #255
	lsls	r0, r0, #8
	ands	r1, r0
	movs	r0, #2
	orrs	r1, r0
.L_08017692:
	adds	r0, r1, #0
	pop	{r4, r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x08017f68
	.4byte 0x04000208
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.2byte 0x7400
	.2byte 0x0200
	push	{r4, r5, r6, lr}
	sub	sp, #64
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #15
	bls.n	.L_080176c4
	ldr	r0, [pc, #0]
	b.n	.L_0801772c
	.2byte 0x80ff
	.2byte 0x0000
.L_080176c4:
	mov	r0, sp
	bl	sub_08017054
	ldr	r2, [pc, #24]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #24]
	ands	r0, r1
	ldr	r1, [pc, #24]
	ldrh	r1, [r1, #36]
	orrs	r0, r1
	strh	r0, [r2, #0]
	lsls	r0, r4, #21
	lsrs	r5, r0, #16
	movs	r6, #0
.L_080176e0:
	movs	r4, #2
	b.n	.L_080176fa
	.4byte 0x04000204
	.4byte 0x0000fffc
	.2byte 0x7f68
	.2byte 0x0801
.L_080176f0:
	subs	r0, r4, #1
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #0
	beq.n	.L_08017708
.L_080176fa:
	adds	r0, r5, #0
	bl	.L_0801762c
	lsls	r0, r0, #16
	lsrs	r3, r0, #16
	cmp	r3, #0
	bne.n	.L_080176f0
.L_08017708:
	adds	r0, r5, #1
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	cmp	r3, #0
	bne.n	.L_0801771c
	adds	r0, r6, #1
	lsls	r0, r0, #16
	lsrs	r6, r0, #16
	cmp	r6, #31
	bls.n	.L_080176e0
.L_0801771c:
	ldr	r2, [pc, #20]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #20]
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
	adds	r0, r3, #0
.L_0801772c:
	add	sp, #64
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x04000204
	.2byte 0xfffc
	.2byte 0x0000
.L_0801773c:
	push	{r4, r5, r6, lr}
	adds	r5, r1, #0
	lsls	r0, r0, #16
	ldr	r3, [pc, #92]
	lsrs	r0, r0, #16
	adds	r4, r0, #0
	ldrb	r1, [r3, #28]
	lsls	r4, r1
	movs	r0, #224
	lsls	r0, r0, #20
	adds	r4, r4, r0
	ldr	r1, [pc, #80]
	ldrh	r0, [r1, #0]
	adds	r6, r0, #0
	movs	r0, #0
	strh	r0, [r1, #0]
	ldr	r2, [pc, #72]
	movs	r0, #170
	strb	r0, [r2, #0]
	ldr	r1, [pc, #72]
	movs	r0, #85
	strb	r0, [r1, #0]
	movs	r0, #160
	strb	r0, [r2, #0]
	ldr	r1, [r3, #24]
	cmp	r1, #0
	beq.n	.L_08017780
.L_08017772:
	ldrb	r0, [r5, #0]
	strb	r0, [r4, #0]
	adds	r5, #1
	adds	r4, #1
	subs	r1, #1
	cmp	r1, #0
	bne.n	.L_08017772
.L_08017780:
	subs	r4, #1
	subs	r5, #1
	ldr	r0, [pc, #28]
	strh	r6, [r0, #0]
	ldr	r0, [pc, #36]
	ldrb	r2, [r5, #0]
	ldr	r3, [r0, #0]
	movs	r0, #1
	adds	r1, r4, #0
	bl	sub_08017884
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x08017f68
	.4byte 0x04000208
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.2byte 0x7400
	.2byte 0x0200
	push	{r4, r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #64
	adds	r7, r1, #0
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #15
	bls.n	.L_080177d0
	ldr	r0, [pc, #4]
	b.n	.L_08017862
	movs	r0, r0
	.2byte 0x80ff
	.2byte 0x0000
.L_080177d0:
	mov	r0, sp
	bl	sub_08017054
	ldr	r2, [pc, #32]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #32]
	ands	r0, r1
	ldr	r1, [pc, #32]
	ldrh	r1, [r1, #36]
	orrs	r0, r1
	strh	r0, [r2, #0]
	lsls	r0, r4, #21
	lsrs	r5, r0, #16
	ldr	r1, [pc, #24]
	ldr	r0, [pc, #24]
	ldr	r0, [r0, #24]
	strh	r0, [r1, #0]
	adds	r0, r1, #0
	mov	r8, r0
	b.n	.L_08017822
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x08017f68
	.4byte 0x0200740c
	.2byte 0x7f3c
	.2byte 0x0801
.L_0801780c:
	ldr	r0, [pc, #32]
	ldr	r1, [r0, #24]
	mov	r2, r8
	ldrh	r2, [r2, #0]
	subs	r0, r2, r1
	mov	r3, r8
	strh	r0, [r3, #0]
	adds	r7, r7, r1
	adds	r0, r5, #1
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
.L_08017822:
	mov	r1, r8
	ldrh	r0, [r1, #0]
	cmp	r0, #0
	beq.n	.L_08017852
	movs	r4, #2
	b.n	.L_0801783e
	movs	r0, r0
	.2byte 0x7f68
	.2byte 0x0801
.L_08017834:
	subs	r0, r4, #1
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #0
	beq.n	.L_0801784e
.L_0801783e:
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	.L_0801773c
	lsls	r0, r0, #16
	lsrs	r6, r0, #16
	cmp	r6, #0
	bne.n	.L_08017834
.L_0801784e:
	cmp	r6, #0
	beq.n	.L_0801780c
.L_08017852:
	ldr	r2, [pc, #28]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #28]
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
	adds	r0, r6, #0
.L_08017862:
	add	sp, #64
	pop	{r3}
	mov	r8, r3
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
