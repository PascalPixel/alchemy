.syntax unified
	.thumb
	.set sub_08006ac0, 0x08006ac0
	.set sub_080072f0, 0x080072f0
	.global Func_08007098
	.thumb_func
Func_08007098:
.L_08007098:
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
	beq.n	.L_080070d8
	movs	r1, #255
.L_080070ce:
	strb	r1, [r4, #0]
	adds	r4, #1
	subs	r0, #1
	cmp	r0, #0
	bne.n	.L_080070ce
.L_080070d8:
	subs	r4, #1
	ldr	r0, [pc, #48]
	strh	r5, [r0, #0]
	ldr	r0, [pc, #56]
	ldr	r3, [r0, #0]
	movs	r0, #1
	adds	r1, r4, #0
	movs	r2, #255
	bl	sub_080072f0
	lsls	r0, r0, #16
	lsrs	r1, r0, #16
	cmp	r1, #0
	beq.n	.L_080070fe
	movs	r0, #255
	lsls	r0, r0, #8
	ands	r1, r0
	movs	r0, #2
	orrs	r1, r0
.L_080070fe:
	adds	r0, r1, #0
	pop	{r4, r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x08007c10
	.4byte 0x04000208
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.2byte 0x4c00
	.2byte 0x0200
	push	{r4, r5, r6, lr}
	sub	sp, #64
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #15
	bls.n	.L_08007130
	ldr	r0, [pc, #0]
	b.n	.L_08007198
	.2byte 0x80ff
	.2byte 0x0000
.L_08007130:
	mov	r0, sp
	bl	sub_08006ac0
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
.L_0800714c:
	movs	r4, #2
	b.n	.L_08007166
	.4byte 0x04000204
	.4byte 0x0000fffc
	.2byte 0x7c10
	.2byte 0x0800
.L_0800715c:
	subs	r0, r4, #1
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #0
	beq.n	.L_08007174
.L_08007166:
	adds	r0, r5, #0
	bl	.L_08007098
	lsls	r0, r0, #16
	lsrs	r3, r0, #16
	cmp	r3, #0
	bne.n	.L_0800715c
.L_08007174:
	adds	r0, r5, #1
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	cmp	r3, #0
	bne.n	.L_08007188
	adds	r0, r6, #1
	lsls	r0, r0, #16
	lsrs	r6, r0, #16
	cmp	r6, #31
	bls.n	.L_0800714c
.L_08007188:
	ldr	r2, [pc, #20]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #20]
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
	adds	r0, r3, #0
.L_08007198:
	add	sp, #64
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x04000204
	.2byte 0xfffc
	.2byte 0x0000
.L_080071a8:
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
	beq.n	.L_080071ec
.L_080071de:
	ldrb	r0, [r5, #0]
	strb	r0, [r4, #0]
	adds	r5, #1
	adds	r4, #1
	subs	r1, #1
	cmp	r1, #0
	bne.n	.L_080071de
.L_080071ec:
	subs	r4, #1
	subs	r5, #1
	ldr	r0, [pc, #28]
	strh	r6, [r0, #0]
	ldr	r0, [pc, #36]
	ldrb	r2, [r5, #0]
	ldr	r3, [r0, #0]
	movs	r0, #1
	adds	r1, r4, #0
	bl	sub_080072f0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x08007c10
	.4byte 0x04000208
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.2byte 0x4c00
	.2byte 0x0200
	push	{r4, r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #64
	adds	r7, r1, #0
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #15
	bls.n	.L_0800723c
	ldr	r0, [pc, #4]
	b.n	.L_080072ce
	movs	r0, r0
	.2byte 0x80ff
	.2byte 0x0000
.L_0800723c:
	mov	r0, sp
	bl	sub_08006ac0
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
	b.n	.L_0800728e
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x08007c10
	.4byte 0x02004c0c
	.2byte 0x7be4
	.2byte 0x0800
.L_08007278:
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
.L_0800728e:
	mov	r1, r8
	ldrh	r0, [r1, #0]
	cmp	r0, #0
	beq.n	.L_080072be
	movs	r4, #2
	b.n	.L_080072aa
	movs	r0, r0
	.2byte 0x7c10
	.2byte 0x0800
.L_080072a0:
	subs	r0, r4, #1
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #0
	beq.n	.L_080072ba
.L_080072aa:
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	.L_080071a8
	lsls	r0, r0, #16
	lsrs	r6, r0, #16
	cmp	r6, #0
	bne.n	.L_080072a0
.L_080072ba:
	cmp	r6, #0
	beq.n	.L_08007278
.L_080072be:
	ldr	r2, [pc, #28]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #28]
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
	adds	r0, r6, #0
.L_080072ce:
	add	sp, #64
	pop	{r3}
	mov	r8, r3
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
