.syntax unified
	.thumb
	.set sub_08014274, 0x08014274
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_08021918, 0x08021918
	.set sub_080229ac, 0x080229ac
	.global Func_08022d1c
	.thumb_func
Func_08022d1c:
.L_08022d1c:
	push	{lr}
	adds	r1, r0, #0
	sub	sp, #4
	cmp	r1, #0
	beq.n	.L_08022d3c
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	movs	r2, #133
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r2, #6
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_08022d3c:
	add	sp, #4
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r2, #0
	mov	r8, r2
	mov	sl, r0
	bl	sub_08021918
	adds	r7, r0, #0
	bl	sub_080143ac
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #16]
	ldrb	r3, [r7, #0]
	adds	r6, r0, #0
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L_08022d6a
	b.n	.L_08022e74
.L_08022d6a:
	ldrb	r3, [r5, #20]
	movs	r2, #0
	b.n	.L_08022d7a
.L_08022d70:
	adds	r2, #1
	adds	r5, #56
	cmp	r2, #63
	bgt.n	.L_08022d80
	ldrb	r3, [r5, #20]
.L_08022d7a:
	cmp	r3, #0
	bne.n	.L_08022d70
	mov	r8, r5
.L_08022d80:
	mov	r3, r8
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_08022e74
	cmp	r6, #96
	beq.n	.L_08022e74
	adds	r0, r6, #0
	movs	r1, #0
	movs	r2, #0
	bl	sub_080142d4
	mov	ip, r0
	cmp	r0, #0
	bne.n	.L_08022da0
	movs	r0, #0
	b.n	.L_08022e74
.L_08022da0:
	mov	r2, r8
	movs	r3, #0
	strh	r3, [r2, #18]
	movs	r3, #1
	strb	r3, [r2, #26]
	ldrb	r3, [r7, #0]
	strb	r6, [r2, #16]
	ldrb	r2, [r7, #1]
	lsls	r3, r3, #8
	adds	r0, r3, r2
	movs	r3, #129
	lsls	r3, r3, #5
	ldr	r1, [pc, #192]
	cmp	r0, r3
	beq.n	.L_08022e2e
	cmp	r0, r3
	bhi.n	.L_08022df2
	movs	r3, #129
	movs	r1, #128
	lsls	r3, r3, #4
	lsls	r1, r1, #8
	cmp	r0, r3
	beq.n	.L_08022e2e
	cmp	r0, r3
	bhi.n	.L_08022dd8
	subs	r3, #8
	movs	r1, #0
	b.n	.L_08022e28
.L_08022dd8:
	movs	r2, #128
	lsls	r2, r2, #5
	movs	r1, #128
	adds	r2, #8
	lsls	r1, r1, #7
	cmp	r0, r2
	beq.n	.L_08022e2e
	movs	r3, #128
	lsls	r3, r3, #5
	movs	r1, #128
	adds	r3, #16
	lsls	r1, r1, #23
	b.n	.L_08022e28
.L_08022df2:
	movs	r3, #129
	lsls	r3, r3, #6
	ldr	r1, [pc, #136]
	cmp	r0, r3
	beq.n	.L_08022e2e
	cmp	r0, r3
	bhi.n	.L_08022e12
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r2, #16
	ldr	r1, [pc, #124]
	cmp	r0, r2
	beq.n	.L_08022e2e
	movs	r1, #128
	subs	r3, #32
	b.n	.L_08022e26
.L_08022e12:
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r2, #32
	ldr	r1, [pc, #108]
	cmp	r0, r2
	beq.n	.L_08022e2e
	movs	r3, #128
	lsls	r3, r3, #7
	movs	r1, #192
	adds	r3, #64
.L_08022e26:
	lsls	r1, r1, #24
.L_08022e28:
	cmp	r0, r3
	beq.n	.L_08022e2e
	movs	r1, #0
.L_08022e2e:
	mov	r4, r8
	movs	r2, #0
	stmia	r4!, {r2}
	movs	r3, #128
	lsls	r3, r3, #6
	orrs	r1, r3
	stmia	r4!, {r1}
	movs	r1, #128
	lsls	r1, r1, #4
	mov	r3, ip
	orrs	r3, r1
	str	r3, [r4, #0]
	mov	r4, r8
	adds	r4, #28
	movs	r3, #192
	str	r2, [r4, #0]
	lsls	r3, r3, #7
	mov	r2, r8
	str	r3, [r2, #32]
	ldr	r3, [pc, #52]
	movs	r2, #187
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	mov	r2, r8
	lsrs	r3, r3, #5
	orrs	r3, r1
	str	r3, [r2, #36]
	adds	r0, r5, #0
	mov	r1, sl
	bl	sub_080229ac
	movs	r3, #1
	negs	r3, r3
	mov	r0, r8
.L_08022e74:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x80008000
	.4byte 0xc0008000
	.4byte 0x80004000
	.4byte 0xc0004000
	.2byte 0x36e0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	sub	sp, #4
	cmp	r7, #0
	beq.n	.L_08022ed4
	ldrb	r2, [r7, #17]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08022eaa
	ldrb	r0, [r7, #16]
	bl	sub_08014274
.L_08022eaa:
	adds	r5, r7, #0
	adds	r5, #40
	movs	r6, #3
.L_08022eb0:
	ldmia	r5!, {r0}
	subs	r6, #1
	bl	.L_08022d1c
	cmp	r6, #0
	bge.n	.L_08022eb0
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	movs	r2, #133
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, r7, #0
	adds	r2, #14
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_08022ed4:
	add	sp, #4
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r5, r2, #0
	bl	sub_08021918
	ldrb	r3, [r0, #5]
	movs	r7, #0
	cmp	r6, r3
	bcc.n	.L_08022eee
	movs	r0, #0
	b.n	.L_08022f20
.L_08022eee:
	ldr	r2, [r0, #16]
	lsls	r3, r6, #2
	ldr	r0, [r3, r2]
.L_08022ef4:
	ldrb	r2, [r0, #0]
	ldrb	r3, [r0, #1]
	adds	r0, #2
	cmp	r2, #254
	beq.n	.L_08022f1e
	cmp	r2, #241
	beq.n	.L_08022f1e
	cmp	r2, #253
	beq.n	.L_08022f1e
	cmp	r2, #239
	beq.n	.L_08022f1e
	cmp	r2, #245
	beq.n	.L_08022f16
	cmp	r2, #255
	beq.n	.L_08022f16
	cmp	r2, #238
	bhi.n	.L_08022ef4
.L_08022f16:
	subs	r5, #1
	adds	r7, r7, r3
	cmp	r5, #0
	bne.n	.L_08022ef4
.L_08022f1e:
	adds	r0, r7, #0
.L_08022f20:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
