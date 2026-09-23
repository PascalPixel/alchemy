.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_08014d00, 0x08014d00
	.set sub_08014d78, 0x08014d78
	.set sub_0803deac, 0x0803deac
	.set sub_0803f2e4, 0x0803f2e4
	.set sub_0803f6c8, 0x0803f6c8
	.global Func_0803db54
	.thumb_func
Func_0803db54:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r0, #0
	mov	sl, r1
	ldr	r5, [pc, #160]
	adds	r0, r5, #0
	bl	sub_08014d78
	adds	r7, r0, #0
	movs	r0, #132
	lsls	r0, r0, #24
	mov	r8, r0
	movs	r3, #128
	lsrs	r5, r5, #2
	mov	r2, r8
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #140]
	adds	r1, r7, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r2, #4
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	adds	r1, r6, #0
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x1c38
	bl	sub_08013164
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_0803dbb8
	ldr	r5, [pc, #104]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r3, #128
	adds	r7, r0, #0
	lsrs	r5, r5, #2
	mov	r2, r8
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #88]
	b.n	.L_0803dbce
.L_0803dbb8:
	ldr	r5, [pc, #88]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r3, #128
	adds	r7, r0, #0
	lsrs	r5, r5, #2
	mov	r2, r8
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #72]
.L_0803dbce:
	adds	r1, r7, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	lsls	r0, r0, #3
	adds	r1, r6, r0
	movs	r2, #192
	movs	r0, #192
	lsls	r2, r2, #3
	lsls	r0, r0, #3
	adds	r3, r6, r2
	adds	r0, #2
	ldrh	r2, [r3, #0]
	adds	r3, r6, r0
	ldrh	r3, [r3, #0]
	adds	r0, r6, #0
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x1c38
	bl	sub_08013164
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000027c
	.4byte 0x08038b14
	.4byte 0x000000a0
	.4byte 0x08038d90
	.4byte 0x00000080
	.2byte 0x8e30
	.2byte 0x0803
	push	{r5, r6, r7, lr}
	movs	r1, #249
	lsls	r1, r1, #2
	movs	r0, #72
	bl	sub_08014d00
	movs	r2, #210
	adds	r7, r0, #0
	lsls	r2, r2, #2
	movs	r5, #0
	adds	r3, r7, r2
	adds	r2, #4
	str	r5, [r3, #0]
	adds	r3, r7, r2
	adds	r2, #4
	str	r5, [r3, #0]
	adds	r3, r7, r2
	adds	r2, #74
	str	r5, [r3, #0]
	adds	r3, r7, r2
	adds	r2, #2
	strh	r5, [r3, #0]
	adds	r3, r7, r2
	strh	r5, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #158
	adds	r2, r7, r3
	movs	r3, #128
	strh	r3, [r2, #0]
	movs	r3, #232
	lsls	r3, r3, #2
	adds	r2, r7, r3
	movs	r3, #32
	strh	r3, [r2, #0]
	movs	r2, #229
	lsls	r2, r2, #2
	adds	r3, r7, r2
	strh	r5, [r3, #0]
	movs	r3, #238
	lsls	r3, r3, #2
	adds	r2, r7, r3
	adds	r3, #47
	strh	r3, [r2, #0]
	movs	r3, #239
	lsls	r3, r3, #1
	adds	r2, r7, r3
	adds	r3, r7, #0
	movs	r1, #0
	movs	r0, #0
	adds	r3, #114
.L_0803dc82:
	adds	r1, #1
	strh	r0, [r3, #0]
	strh	r0, [r2, #0]
	adds	r3, #52
	adds	r2, #52
	cmp	r1, #5
	bne.n	.L_0803dc82
	movs	r2, #186
	lsls	r2, r2, #1
	movs	r5, #0
	adds	r3, r7, r2
	adds	r2, #52
	strh	r5, [r3, #2]
	adds	r3, r7, r2
	strh	r5, [r3, #2]
	adds	r3, r7, #0
	adds	r3, #70
	strh	r5, [r7, #10]
	strh	r5, [r7, #62]
	strh	r5, [r7, #18]
	strh	r5, [r3, #0]
	ldr	r6, [pc, #112]
	bl	sub_080143ac
	movs	r2, #185
	lsls	r2, r2, #2
	adds	r3, r7, r2
	strh	r0, [r3, #0]
	movs	r1, #128
	adds	r2, r6, #0
	ldrh	r0, [r3, #0]
	lsls	r1, r1, #1
	bl	sub_080142d4
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #230
	adds	r3, r7, r2
	subs	r2, #4
	strh	r0, [r3, #0]
	adds	r3, r7, r2
	adds	r2, #24
	strh	r5, [r3, #0]
	adds	r3, r7, r2
	adds	r2, #28
	strh	r5, [r3, #0]
	adds	r3, r7, r2
	strh	r5, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r5, r7, r3
	ldrb	r3, [r5, #5]
	movs	r0, #13
	negs	r0, r0
	adds	r2, r0, #0
	ands	r2, r3
	movs	r3, #17
	negs	r3, r3
	ands	r2, r3
	movs	r3, #32
	ldrb	r1, [r5, #7]
	orrs	r2, r3
	movs	r3, #4
	negs	r3, r3
	ands	r2, r3
	subs	r3, #59
	movs	r4, #63
	ands	r3, r1
	ands	r3, r4
	movs	r1, #64
	orrs	r3, r1
	strb	r3, [r5, #7]
	ldrb	r3, [r5, #9]
	ands	r2, r4
	ands	r0, r3
	strb	r2, [r5, #5]
	strb	r0, [r5, #9]
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xc9c4
	.2byte 0x0805
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #72]
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r1, r2
	movs	r0, #0
	adds	r2, #82
	str	r0, [r3, #0]
	adds	r3, r1, r2
	strh	r0, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #158
	adds	r4, r1, r3
	ldrh	r2, [r4, #0]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803dd58
	movs	r2, #231
	lsls	r2, r2, #2
	adds	r3, r1, r2
	strh	r0, [r3, #0]
	strh	r0, [r4, #0]
.L_0803dd58:
	movs	r2, #232
	lsls	r2, r2, #2
	adds	r3, r1, r2
	subs	r2, #12
	strh	r0, [r3, #0]
	adds	r3, r1, r2
	strh	r0, [r3, #0]
	pop	{pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #72]
	movs	r3, #229
	lsls	r3, r3, #2
	adds	r4, r2, r3
	ldrh	r3, [r4, #0]
	cmp	r3, #16
	beq.n	.L_0803dd96
	lsls	r3, r3, #1
	mov	ip, r3
	movs	r3, #213
	lsls	r3, r3, #2
	add	r3, ip
	strh	r0, [r2, r3]
	movs	r3, #221
	lsls	r3, r3, #2
	add	r3, ip
	strh	r1, [r2, r3]
	ldrh	r3, [r4, #0]
	adds	r3, #1
	strh	r3, [r4, #0]
.L_0803dd96:
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #72]
	sub	sp, #8
	mov	r9, r3
	movs	r3, #229
	lsls	r3, r3, #2
	add	r3, r9
	ldrh	r3, [r3, #0]
	movs	r2, #213
	str	r3, [sp, #4]
	movs	r3, #231
	lsls	r3, r3, #2
	add	r3, r9
	ldrh	r3, [r3, #0]
	movs	r6, #0
	mov	sl, r3
	lsls	r3, r3, #1
	add	r3, r9
	lsls	r2, r2, #2
	mov	fp, r6
	adds	r4, r3, r2
	b.n	.L_0803de0e
.L_0803ddd4:
	adds	r2, r5, #0
	movs	r3, #0
	adds	r0, r7, #0
	mov	r1, r8
	str	r4, [sp, #0]
	bl	sub_0803f2e4
	movs	r3, #210
	lsls	r3, r3, #2
	add	r3, r9
	ldr	r2, [r3, #0]
	ldr	r4, [sp, #0]
	cmp	r2, #0
	bne.n	.L_0803ddf6
	str	r5, [r3, #0]
	str	r2, [r5, #0]
	b.n	.L_0803ddfa
.L_0803ddf6:
	str	r5, [r6, #4]
	str	r6, [r5, #0]
.L_0803ddfa:
	movs	r3, #0
	str	r3, [r5, #4]
	movs	r3, #1
	add	fp, r3
	mov	r2, fp
	adds	r6, r5, #0
	cmp	r2, #5
	beq.n	.L_0803de2a
	adds	r4, #2
	add	sl, r3
.L_0803de0e:
	ldr	r3, [sp, #4]
	cmp	sl, r3
	bcs.n	.L_0803de2a
	ldrh	r2, [r4, #32]
	movs	r0, #0
	ldrh	r7, [r4, #0]
	str	r4, [sp, #0]
	mov	r8, r2
	bl	sub_0803deac
	adds	r5, r0, #0
	ldr	r4, [sp, #0]
	cmp	r5, #0
	bne.n	.L_0803ddd4
.L_0803de2a:
	mov	r3, fp
	lsls	r2, r3, #3
	ldr	r3, [pc, #52]
	movs	r1, #192
	lsls	r1, r1, #2
	subs	r3, r3, r2
	adds	r1, #150
	movs	r2, #230
	add	r1, r9
	lsls	r2, r2, #2
	strh	r3, [r1, #0]
	add	r2, r9
	movs	r3, #140
	strh	r3, [r2, #0]
	movs	r3, #210
	lsls	r3, r3, #2
	add	r3, r9
	ldr	r6, [r3, #0]
	movs	r3, #0
	mov	fp, r3
	cmp	r6, #0
	beq.n	.L_0803de94
	movs	r0, #238
	lsls	r0, r0, #2
	adds	r5, r1, #0
	adds	r4, r2, #0
	add	r0, r9
	movs	r1, #0
	b.n	.L_0803de68
	.2byte 0x0064
	.2byte 0x0000
.L_0803de68:
	ldrh	r3, [r5, #0]
	add	r3, fp
	strh	r3, [r6, #16]
	ldrh	r2, [r4, #0]
	strh	r3, [r6, #24]
	strh	r2, [r6, #18]
	strh	r2, [r6, #26]
	ldrh	r2, [r6, #10]
	cmp	r2, #6
	bne.n	.L_0803de86
	ldrh	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_0803de86
	strh	r2, [r6, #18]
	strh	r2, [r6, #26]
.L_0803de86:
	strh	r1, [r6, #20]
	strh	r1, [r6, #22]
	ldr	r6, [r6, #4]
	movs	r2, #16
	add	fp, r2
	cmp	r6, #0
	bne.n	.L_0803de68
.L_0803de94:
	bl	sub_0803f6c8
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.align 2, 0
