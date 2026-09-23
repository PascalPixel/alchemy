.syntax unified
	.thumb
	.set sub_0801456c, 0x0801456c
	.set sub_080145a8, 0x080145a8
	.set sub_08014694, 0x08014694
	.set sub_0801475c, 0x0801475c
	.global Func_0802cc9c
	.thumb_func
Func_0802cc9c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #32]
	adds	r5, r0, #0
	adds	r0, r6, #0
	movs	r1, #192
	adds	r0, #24
	ldr	r3, [pc, #144]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x8829
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	movs	r7, #0
	adds	r5, #2
	cmp	r1, r2
	beq.n	.L_0802cd1c
	movs	r3, #255
	movs	r2, #253
	lsls	r3, r3, #8
	lsls	r2, r2, #8
	mov	sl, r3
	mov	r8, r2
	movs	r3, #15
	movs	r2, #128
	mov	lr, r3
	mov	ip, r2
	movs	r4, #0
.L_0802ccde:
	adds	r3, r1, #0
	mov	r2, sl
	ands	r3, r2
	cmp	r3, r8
	bne.n	.L_0802cd0e
	mov	r3, lr
	adds	r2, r1, #0
	ands	r2, r3
	mov	r3, ip
	ands	r3, r1
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_0802ccfa
	movs	r0, #1
.L_0802ccfa:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r6, r3
	adds	r3, #24
	str	r5, [r3, #0]
	str	r5, [r3, #4]
	strh	r4, [r3, #8]
	strh	r0, [r3, #10]
	adds	r7, #1
.L_0802cd0e:
	ldrh	r1, [r5, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r5, #2
	cmp	r1, r2
	bne.n	.L_0802ccde
.L_0802cd1c:
	cmp	r7, #0
	beq.n	.L_0802cd3a
	ldr	r5, [pc, #36]
	adds	r0, r5, #0
	bl	sub_0801456c
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_0802cd3a
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r0, r5, #0
	bl	sub_080145a8
.L_0802cd3a:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03000258
	.2byte 0xcb65
	.2byte 0x0802
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_0802cd6a
	ldr	r0, [pc, #4]
	bl	sub_08014694
.L_0802cd6a:
	pop	{pc}
	.2byte 0xcb65
	.2byte 0x0802
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_0802cd8e
	ldr	r0, [pc, #4]
	bl	sub_0801475c
.L_0802cd8e:
	pop	{pc}
	.2byte 0xcb65
	.2byte 0x0802
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	mov	ip, r3
	mov	r4, ip
	adds	r4, #216
	ldr	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L_0802ce48
	ldrh	r3, [r4, #10]
	cmp	r3, #0
	bne.n	.L_0802ce48
.L_0802cdae:
	ldrh	r2, [r4, #8]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_0802ce3e
	ldr	r0, [r4, #4]
	movs	r2, #255
	ldrh	r1, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r0, #2
	cmp	r1, r2
	bne.n	.L_0802cdcc
	ldr	r3, [r4, #0]
	str	r3, [r4, #4]
	b.n	.L_0802cdae
.L_0802cdcc:
	movs	r3, #255
	lsls	r3, r3, #8
	movs	r2, #254
	ands	r3, r1
	lsls	r2, r2, #8
	cmp	r3, r2
	bne.n	.L_0802cdec
	movs	r2, #255
	ands	r2, r1
	cmp	r2, #255
	beq.n	.L_0802ce48
	ldr	r3, [r4, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r4, #4]
	b.n	.L_0802cdae
.L_0802cdec:
	movs	r3, #240
	lsls	r3, r3, #8
	movs	r2, #192
	ands	r3, r1
	lsls	r2, r2, #6
	cmp	r3, r2
	bne.n	.L_0802ce12
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	strh	r1, [r3, #0]
	movs	r3, #8
	adds	r3, #255
	add	r3, ip
	strb	r1, [r3, #0]
	ldr	r3, [r4, #4]
	adds	r3, #2
	str	r3, [r4, #4]
	b.n	.L_0802cdae
.L_0802ce12:
	movs	r3, #8
	adds	r3, #255
	add	r3, ip
	ldrb	r2, [r3, #0]
	movs	r3, #192
	ands	r3, r2
	cmp	r3, #64
	bne.n	.L_0802ce2a
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	b.n	.L_0802ce30
.L_0802ce2a:
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #84
.L_0802ce30:
	strh	r1, [r3, #0]
	ldrh	r3, [r0, #0]
	strh	r3, [r4, #8]
	ldr	r3, [r4, #4]
	adds	r3, #4
	str	r3, [r4, #4]
	b.n	.L_0802cdae
.L_0802ce3e:
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r2, r1
	strh	r3, [r4, #8]
.L_0802ce48:
	pop	{pc}
	.2byte 0x0000
