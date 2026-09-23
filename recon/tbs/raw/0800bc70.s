.syntax unified
	.thumb
	.set sub_08003fa4, 0x08003fa4
	.set sub_08004080, 0x08004080
	.set sub_0800b8ac, 0x0800b8ac
	.set sub_08185000, 0x08185000
	.global Func_0800bc70
	.thumb_func
Func_0800bc70:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #0
	mov	r8, r1
	mov	sl, r0
	bl	sub_08185000
	adds	r7, r0, #0
	bl	sub_08004080
	ldr	r3, [pc, #284]
	ldr	r5, [r3, #0]
	ldrb	r3, [r7, #0]
	adds	r6, r0, #0
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L_0800bc98
	b.n	.L_0800bd9c
.L_0800bc98:
	adds	r3, r5, #0
	adds	r3, #32
	ldrb	r3, [r3, #0]
	movs	r2, #0
	b.n	.L_0800bcb0
.L_0800bca2:
	adds	r2, #1
	adds	r5, #56
	cmp	r2, #63
	bgt.n	.L_0800bcb6
	adds	r3, r5, #0
	adds	r3, #32
	ldrb	r3, [r3, #0]
.L_0800bcb0:
	cmp	r3, #0
	bne.n	.L_0800bca2
	mov	r8, r5
.L_0800bcb6:
	mov	r2, r8
	movs	r0, #0
	cmp	r2, #0
	beq.n	.L_0800bd9c
	cmp	r6, #96
	beq.n	.L_0800bd9c
	adds	r0, r6, #0
	movs	r1, #0
	movs	r2, #0
	bl	sub_08003fa4
	mov	ip, r0
	cmp	r0, #0
	bne.n	.L_0800bcd6
	movs	r0, #0
	b.n	.L_0800bd9c
.L_0800bcd6:
	movs	r3, #0
	mov	r2, r8
	mov	r1, r8
	strb	r6, [r1, #28]
	strh	r3, [r2, #30]
	adds	r2, #38
	movs	r3, #1
	strb	r3, [r2, #0]
	ldrb	r3, [r7, #0]
	ldrb	r2, [r7, #1]
	lsls	r3, r3, #8
	adds	r0, r3, r2
	movs	r3, #129
	lsls	r3, r3, #5
	ldr	r4, [pc, #184]
	cmp	r0, r3
	beq.n	.L_0800bd5e
	cmp	r0, r3
	bhi.n	.L_0800bd28
	movs	r3, #129
	movs	r4, #128
	lsls	r3, r3, #4
	lsls	r4, r4, #8
	cmp	r0, r3
	beq.n	.L_0800bd5e
	cmp	r0, r3
	bhi.n	.L_0800bd12
	subs	r3, #8
	movs	r4, #0
	b.n	.L_0800bd58
.L_0800bd12:
	ldr	r1, [pc, #156]
	movs	r4, #128
	lsls	r4, r4, #7
	cmp	r0, r1
	beq.n	.L_0800bd5e
	ldr	r2, [pc, #148]
	movs	r4, #128
	lsls	r4, r4, #23
	cmp	r0, r2
	beq.n	.L_0800bd5e
	b.n	.L_0800bd5c
.L_0800bd28:
	movs	r3, #129
	lsls	r3, r3, #6
	ldr	r4, [pc, #136]
	cmp	r0, r3
	beq.n	.L_0800bd5e
	cmp	r0, r3
	bhi.n	.L_0800bd4a
	subs	r3, #48
	ldr	r4, [pc, #128]
	cmp	r0, r3
	beq.n	.L_0800bd5e
	ldr	r1, [pc, #128]
	movs	r4, #128
	lsls	r4, r4, #24
	cmp	r0, r1
	beq.n	.L_0800bd5e
	b.n	.L_0800bd5c
.L_0800bd4a:
	ldr	r2, [pc, #120]
	ldr	r4, [pc, #120]
	cmp	r0, r2
	beq.n	.L_0800bd5e
	ldr	r3, [pc, #120]
	movs	r4, #192
	lsls	r4, r4, #24
.L_0800bd58:
	cmp	r0, r3
	beq.n	.L_0800bd5e
.L_0800bd5c:
	movs	r4, #0
.L_0800bd5e:
	mov	r2, r8
	movs	r1, #0
	movs	r3, #128
	stmia	r2!, {r1}
	lsls	r3, r3, #6
	orrs	r4, r3
	movs	r0, #128
	stmia	r2!, {r4}
	lsls	r0, r0, #4
	mov	r3, ip
	orrs	r3, r0
	stmia	r2!, {r3}
	movs	r3, #192
	stmia	r2!, {r1}
	lsls	r3, r3, #7
	stmia	r2!, {r3}
	movs	r1, #187
	ldr	r3, [pc, #76]
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrh	r3, [r3, #0]
	lsrs	r3, r3, #5
	orrs	r3, r0
	str	r3, [r2, #0]
	adds	r0, r5, #0
	mov	r1, sl
	bl	sub_0800b8ac
	movs	r2, #1
	negs	r2, r2
	mov	r0, r8
.L_0800bd9c:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e60
	.4byte 0x80008000
	.4byte 0x00001008
	.4byte 0x00001010
	.4byte 0xc0008000
	.4byte 0x80004000
	.4byte 0x00002020
	.4byte 0x00004020
	.4byte 0xc0004000
	.4byte 0x00004040
	.4byte 0x03001b10
