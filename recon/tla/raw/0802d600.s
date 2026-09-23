.syntax unified
	.thumb
	.set sub_0802d45c, 0x0802d45c
	.set sub_080c8730, 0x080c8730
	.global Func_0802d600
	.thumb_func
Func_0802d600:
	push	{r5, r6, lr}
	movs	r4, #192
	movs	r5, #3
	ands	r5, r2
	lsls	r4, r4, #18
	ldr	r6, [r4, #32]
	lsls	r4, r5, #3
	subs	r4, r4, r5
	movs	r5, #156
	lsls	r5, r5, #1
	lsls	r4, r4, #3
	adds	r4, r4, r5
	ldr	r5, [r6, r4]
	ldr	r4, [pc, #32]
	asrs	r1, r1, #20
	adds	r5, r5, r4
	ldr	r4, [pc, #28]
	lsls	r1, r1, #7
	asrs	r5, r5, #2
	asrs	r0, r0, #20
	adds	r5, r5, r4
	adds	r0, r0, r1
	adds	r5, r5, r0
	movs	r1, #0
	adds	r0, r2, #0
	adds	r2, r3, #0
	bl	sub_080c8730
	strb	r0, [r5, #0]
	pop	{r5, r6, pc}
	.4byte 0xfdff0000
	.2byte 0x4000
	.2byte 0x0202
	push	{lr}
	adds	r3, r0, #0
	adds	r4, r1, #0
	adds	r0, r2, #0
	adds	r1, r3, #0
	adds	r2, r4, #0
	bl	sub_0802d45c
	asrs	r0, r0, #19
	pop	{pc}
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r5, r0, #0
	ldr	r0, [r3, #32]
	adds	r4, r2, #0
	asrs	r1, r1, #16
	asrs	r4, r4, #16
	cmp	r0, #0
	beq.n	.L_0802d680
	movs	r2, #3
	ands	r2, r5
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #158
	lsls	r3, r3, #3
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r0, r3]
	b.n	.L_0802d682
.L_0802d680:
	ldr	r0, [pc, #36]
.L_0802d682:
	cmp	r1, #0
	bge.n	.L_0802d688
	adds	r1, #15
.L_0802d688:
	adds	r2, r4, #0
	asrs	r1, r1, #4
	cmp	r2, #0
	bge.n	.L_0802d692
	adds	r2, #15
.L_0802d692:
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
	asrs	r1, r1, #20
	asrs	r4, r4, #20
	ldr	r2, [pc, #32]
	cmp	r0, #0
	beq.n	.L_0802d6d8
	movs	r2, #3
	ands	r2, r5
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [r0, r3]
.L_0802d6d8:
	lsls	r3, r4, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrb	r0, [r2, #2]
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, r6, lr}
	adds	r6, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r5, r0, #0
	ldr	r0, [r3, #32]
	adds	r4, r2, #0
	asrs	r1, r1, #20
	asrs	r4, r4, #20
	cmp	r0, #0
	beq.n	.L_0802d71a
	movs	r2, #3
	ands	r2, r5
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [r0, r3]
	lsls	r3, r4, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	strb	r6, [r2, #2]
.L_0802d71a:
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	adds	r7, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	asrs	r6, r1, #16
	ldr	r1, [r3, #32]
	asrs	r5, r0, #16
	ldr	r3, [r3, #108]
	movs	r0, #0
	cmp	r1, #0
	beq.n	.L_0802d79e
	movs	r0, #197
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r0, #0
	cmp	r3, #3
	beq.n	.L_0802d79e
	cmp	r2, #2
	bgt.n	.L_0802d758
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #158
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [r1, r3]
	b.n	.L_0802d75a
.L_0802d758:
	ldr	r2, [pc, #68]
.L_0802d75a:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_0802d762
	adds	r3, #15
.L_0802d762:
	asrs	r1, r3, #4
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_0802d76c
	adds	r3, #15
.L_0802d76c:
	asrs	r3, r3, #4
	lsls	r3, r3, #7
	adds	r3, r1, r3
	ldrb	r1, [r2, r3]
	ldr	r3, [pc, #44]
	lsls	r1, r1, #2
	adds	r0, r1, r3
	ldrb	r0, [r0, #0]
	movs	r2, #15
	adds	r3, r2, #0
	ands	r3, r0
	lsls	r3, r3, #2
	mov	ip, r3
	ldr	r4, [pc, #32]
	ldr	r3, [pc, #32]
	ands	r5, r2
	ands	r6, r2
	mov	r2, ip
	adds	r0, r1, r3
	ldr	r3, [r4, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1bc0
.L_0802d79e:
	pop	{r5, r6, r7, pc}
	.4byte 0x02024000
	.4byte 0x0202c000
	.4byte 0x0802efc4
	.2byte 0xc001
	.2byte 0x0202
	push	{r5, r6, r7, lr}
	adds	r7, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	asrs	r6, r1, #16
	ldr	r1, [r3, #32]
	asrs	r5, r0, #16
	ldr	r3, [r3, #108]
	movs	r0, #0
	cmp	r1, #0
	beq.n	.L_0802d862
	movs	r0, #197
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r0, #0
	cmp	r3, #3
	beq.n	.L_0802d862
	cmp	r2, #2
	bgt.n	.L_0802d7f2
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r4, #156
	lsls	r2, r3, #3
	lsls	r4, r4, #1
	adds	r3, r2, r4
	adds	r4, #4
	ldr	r0, [r1, r3]
	adds	r3, r2, r4
	ldr	r1, [r1, r3]
	b.n	.L_0802d7f6
.L_0802d7f2:
	ldr	r0, [pc, #112]
	ldr	r1, [pc, #112]
.L_0802d7f6:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_0802d7fe
	adds	r3, #15
.L_0802d7fe:
	asrs	r2, r3, #4
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_0802d808
	adds	r3, #15
.L_0802d808:
	asrs	r3, r3, #4
	lsls	r3, r3, #7
	adds	r3, r2, r3
	lsls	r2, r3, #2
	adds	r0, r0, r2
	adds	r1, r1, r3
	ldrb	r3, [r0, #2]
	movs	r0, #2
	cmp	r3, #255
	beq.n	.L_0802d862
	ldrb	r1, [r1, #0]
	ldr	r2, [pc, #76]
	lsls	r1, r1, #2
	adds	r0, r1, r2
	ldrb	r0, [r0, #0]
	movs	r2, #15
	adds	r3, r2, #0
	ands	r3, r0
	lsls	r3, r3, #2
	mov	ip, r3
	ldr	r4, [pc, #60]
	ldr	r3, [pc, #64]
	ands	r5, r2
	ands	r6, r2
	mov	r2, ip
	adds	r0, r1, r3
	ldr	r3, [r4, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2380
	subs	r0, r0, r7
	lsls	r3, r3, #12
	cmp	r0, r3
	ble.n	.L_0802d854
	movs	r0, #1
	b.n	.L_0802d862
.L_0802d854:
	ldr	r4, [pc, #32]
	cmp	r0, r4
	bge.n	.L_0802d860
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0802d862
.L_0802d860:
	movs	r0, #0
.L_0802d862:
	pop	{r5, r6, r7, pc}
	.4byte 0x02010000
	.4byte 0x02024000
	.4byte 0x0202c000
	.4byte 0x0802efc4
	.4byte 0x0202c001
	.2byte 0x0000
	.2byte 0xfff4
	.2byte 0xb5e0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r7, r0, #0
	movs	r2, #10
	ldrsh	r6, [r1, r2]
	movs	r0, #2
	ldrsh	r5, [r1, r0]
	ldr	r1, [r3, #32]
	movs	r0, #0
	ldr	r3, [r3, #108]
	cmp	r1, #0
	beq.n	.L_0802d93c
	movs	r4, #197
	lsls	r4, r4, #1
	adds	r3, r3, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	beq.n	.L_0802d93c
	adds	r2, r7, #0
	adds	r2, #34
	ldrb	r3, [r2, #0]
	cmp	r3, #2
	bhi.n	.L_0802d8ca
	adds	r2, r3, #0
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r0, #156
	lsls	r2, r3, #3
	lsls	r0, r0, #1
	movs	r4, #158
	adds	r3, r2, r0
	lsls	r4, r4, #1
	ldr	r0, [r1, r3]
	adds	r3, r2, r4
	ldr	r1, [r1, r3]
	b.n	.L_0802d8ce
.L_0802d8ca:
	ldr	r0, [pc, #116]
	ldr	r1, [pc, #116]
.L_0802d8ce:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_0802d8d6
	adds	r3, #15
.L_0802d8d6:
	asrs	r2, r3, #4
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_0802d8e0
	adds	r3, #15
.L_0802d8e0:
	asrs	r3, r3, #4
	lsls	r3, r3, #7
	adds	r3, r2, r3
	lsls	r2, r3, #2
	adds	r0, r0, r2
	adds	r1, r1, r3
	ldrb	r3, [r0, #2]
	movs	r0, #2
	cmp	r3, #255
	beq.n	.L_0802d93c
	ldrb	r1, [r1, #0]
	ldr	r2, [pc, #80]
	lsls	r1, r1, #2
	adds	r0, r1, r2
	ldrb	r0, [r0, #0]
	movs	r2, #15
	adds	r3, r2, #0
	ands	r3, r0
	lsls	r3, r3, #2
	mov	ip, r3
	ldr	r4, [pc, #64]
	ldr	r3, [pc, #68]
	ands	r5, r2
	ands	r6, r2
	mov	r2, ip
	adds	r0, r1, r3
	ldr	r3, [r4, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x697b
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	ble.n	.L_0802d92e
	movs	r0, #1
	b.n	.L_0802d93c
.L_0802d92e:
	ldr	r4, [pc, #36]
	cmp	r0, r4
	bge.n	.L_0802d93a
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0802d93c
.L_0802d93a:
	movs	r0, #0
.L_0802d93c:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x02024000
	.4byte 0x0202c000
	.4byte 0x0802efc4
	.4byte 0x0202c001
	.2byte 0x0000
	.2byte 0xfff4
	.2byte 0xb5e0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r7, r0, #0
	movs	r2, #10
	ldrsh	r6, [r1, r2]
	movs	r0, #2
	ldrsh	r5, [r1, r0]
	ldr	r1, [r3, #32]
	movs	r0, #0
	ldr	r3, [r3, #108]
	cmp	r1, #0
	beq.n	.L_0802da0c
	movs	r4, #197
	lsls	r4, r4, #1
	adds	r3, r3, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	beq.n	.L_0802da0c
	adds	r2, r7, #0
	adds	r2, #34
	ldrb	r3, [r2, #0]
	cmp	r3, #2
	bhi.n	.L_0802d9a6
	adds	r2, r3, #0
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r0, #156
	lsls	r2, r3, #3
	lsls	r0, r0, #1
	movs	r4, #158
	adds	r3, r2, r0
	lsls	r4, r4, #1
	ldr	r0, [r1, r3]
	adds	r3, r2, r4
	ldr	r1, [r1, r3]
	b.n	.L_0802d9aa
.L_0802d9a6:
	ldr	r0, [pc, #104]
	ldr	r1, [pc, #104]
.L_0802d9aa:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_0802d9b2
	adds	r3, #15
.L_0802d9b2:
	asrs	r2, r3, #4
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_0802d9bc
	adds	r3, #15
.L_0802d9bc:
	asrs	r3, r3, #4
	lsls	r3, r3, #7
	adds	r3, r2, r3
	lsls	r2, r3, #2
	adds	r0, r0, r2
	adds	r1, r1, r3
	ldrb	r3, [r0, #2]
	movs	r0, #2
	cmp	r3, #255
	beq.n	.L_0802da0c
	ldrb	r1, [r1, #0]
	ldr	r2, [pc, #68]
	lsls	r1, r1, #2
	adds	r0, r1, r2
	ldrb	r0, [r0, #0]
	movs	r2, #15
	adds	r3, r2, #0
	ands	r3, r0
	lsls	r3, r3, #2
	mov	ip, r3
	ldr	r4, [pc, #52]
	ldr	r3, [pc, #56]
	ands	r5, r2
	ands	r6, r2
	mov	r2, ip
	adds	r0, r1, r3
	ldr	r3, [r4, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x68fb
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	ble.n	.L_0802da0a
	movs	r0, #1
	b.n	.L_0802da0c
.L_0802da0a:
	movs	r0, #0
.L_0802da0c:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x02024000
	.4byte 0x0202c000
	.4byte 0x0802efc4
	.2byte 0xc001
	.2byte 0x0202
	push	{lr}
	ldr	r3, [r0, #0]
	cmp	r3, #0
	bge.n	.L_0802da34
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r3, r3, r2
.L_0802da34:
	ldr	r2, [pc, #76]
	asrs	r1, r3, #16
	ldr	r3, [r0, #8]
	ands	r3, r2
	ldr	r2, [r0, #4]
	subs	r3, r3, r2
	lsrs	r0, r3, #16
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	cmp	r3, #0
	bne.n	.L_0802da50
	movs	r0, #0
	b.n	.L_0802da80
.L_0802da50:
	movs	r2, #212
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [r3, #0]
	adds	r3, r1, #0
	cmp	r3, #0
	bge.n	.L_0802da60
	adds	r3, #15
.L_0802da60:
	asrs	r1, r3, #4
	cmp	r0, #0
	bge.n	.L_0802da68
	adds	r0, #15
.L_0802da68:
	asrs	r3, r0, #4
	lsls	r3, r3, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrb	r3, [r2, #2]
	movs	r2, #255
	eors	r3, r2
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	subs	r0, #1
.L_0802da80:
	pop	{pc}
	movs	r0, r0
	.2byte 0x0000
	.2byte 0xfff0
.L_0802da88:
	.2byte 0xb500
	ldr	r3, [r0, #8]
	ldr	r0, [r0, #0]
	cmp	r0, #0
	bge.n	.L_0802da96
	ldr	r1, [pc, #36]
	adds	r0, r0, r1
.L_0802da96:
	asrs	r0, r0, #21
	movs	r2, #31
	ands	r0, r2
	cmp	r3, #0
	bge.n	.L_0802daa4
	ldr	r1, [pc, #20]
	adds	r3, r3, r1
.L_0802daa4:
	asrs	r3, r3, #21
	ands	r3, r2
	lsls	r3, r3, #5
	ldr	r2, [pc, #16]
	adds	r3, r0, r3
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrb	r0, [r3, #2]
	pop	{pc}
	movs	r0, r0
	.4byte 0x001fffff
	.2byte 0x0000
	.2byte 0x0202
.L_0802dac0:
	push	{r5, lr}
	ldr	r4, [r0, #8]
	ldr	r3, [r0, #0]
	asrs	r2, r4, #17
	asrs	r1, r3, #17
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_0802dad2
	adds	r3, r2, #7
.L_0802dad2:
	asrs	r3, r3, #3
	movs	r5, #63
	ands	r3, r5
	lsls	r0, r3, #6
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_0802dae2
	adds	r3, r1, #7
.L_0802dae2:
	asrs	r3, r3, #3
	ands	r3, r5
	adds	r5, r0, r3
	lsrs	r3, r4, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	movs	r2, #3
	ands	r3, r2
	lsls	r0, r3, #1
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_0802dafc
	adds	r3, r1, #3
.L_0802dafc:
	movs	r2, #1
	asrs	r3, r3, #2
	ands	r3, r2
	ldr	r2, [pc, #80]
	adds	r4, r0, r3
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	ldr	r2, [pc, #76]
	lsls	r3, r3, #3
	adds	r3, r3, r4
	adds	r3, r3, r2
	ldrb	r2, [r3, #0]
	cmp	r2, #0
	beq.n	.L_0802db2a
	movs	r3, #2
	ands	r3, r1
	lsrs	r0, r2, #4
	cmp	r3, #0
	bne.n	.L_0802db26
	movs	r0, #15
	ands	r0, r2
.L_0802db26:
	cmp	r0, #0
	bne.n	.L_0802db52
.L_0802db2a:
	ldr	r2, [pc, #48]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	ldr	r2, [pc, #44]
	lsls	r3, r3, #3
	adds	r3, r3, r4
	adds	r3, r3, r2
	ldrb	r2, [r3, #0]
	cmp	r2, #0
	beq.n	.L_0802db50
	movs	r3, #2
	ands	r3, r1
	lsrs	r0, r2, #4
	cmp	r3, #0
	bne.n	.L_0802db4c
	movs	r0, #15
	ands	r0, r2
.L_0802db4c:
	cmp	r0, #0
	bne.n	.L_0802db52
.L_0802db50:
	movs	r0, #7
.L_0802db52:
	pop	{r5, pc}
	.4byte 0x06005000
	.4byte 0x0202c800
	.4byte 0x06004000
	.2byte 0xc000
	.2byte 0x0202
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	bl	.L_0802da88
	cmp	r0, #255
	beq.n	.L_0802db82
	adds	r0, r5, #0
	bl	.L_0802dac0
	subs	r0, #5
	cmp	r0, #7
	bhi.n	.L_0802db82
	movs	r0, #0
	b.n	.L_0802db86
.L_0802db82:
	movs	r0, #1
	negs	r0, r0
.L_0802db86:
	pop	{r5, pc}
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	bl	.L_0802da88
	cmp	r0, #255
	beq.n	.L_0802dbca
	adds	r0, r5, #0
	bl	.L_0802dac0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #58
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_0802dbbc
	cmp	r3, #1
	beq.n	.L_0802dbbc
	cmp	r3, #2
	bne.n	.L_0802dbca
.L_0802dbbc:
	subs	r3, r0, #1
	cmp	r3, #3
	bls.n	.L_0802dbc6
	cmp	r0, #6
	bne.n	.L_0802dbca
.L_0802dbc6:
	movs	r0, #0
	b.n	.L_0802dbce
.L_0802dbca:
	movs	r0, #1
	negs	r0, r0
.L_0802dbce:
	pop	{r5, pc}
	push	{r5, r6, lr}
	adds	r5, r1, #0
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	.L_0802da88
	cmp	r0, #255
	beq.n	.L_0802dc40
	adds	r0, r5, #0
	bl	.L_0802dac0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	ldr	r3, [r6, #12]
	movs	r1, #128
	lsls	r1, r1, #11
	cmp	r3, r1
	bge.n	.L_0802dc02
	subs	r3, r0, #1
	cmp	r3, #3
	bls.n	.L_0802dc1a
	cmp	r0, #6
	beq.n	.L_0802dc1a
	b.n	.L_0802dc40
.L_0802dc02:
	movs	r1, #208
	lsls	r1, r1, #4
	adds	r1, #58
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_0802dc1e
	subs	r3, r0, #1
	cmp	r3, #5
	bhi.n	.L_0802dc40
.L_0802dc1a:
	movs	r0, #0
	b.n	.L_0802dc44
.L_0802dc1e:
	cmp	r3, #1
	bne.n	.L_0802dc2a
	subs	r3, r0, #1
	cmp	r3, #7
	bhi.n	.L_0802dc40
	b.n	.L_0802dc1a
.L_0802dc2a:
	cmp	r3, #2
	bne.n	.L_0802dc36
	subs	r3, r0, #1
	cmp	r3, #7
	bhi.n	.L_0802dc3c
	b.n	.L_0802dc1a
.L_0802dc36:
	subs	r3, r0, #4
	cmp	r3, #4
	bls.n	.L_0802dc1a
.L_0802dc3c:
	cmp	r0, #10
	beq.n	.L_0802dc1a
.L_0802dc40:
	movs	r0, #1
	negs	r0, r0
.L_0802dc44:
	pop	{r5, r6, pc}
	.align 2, 0
