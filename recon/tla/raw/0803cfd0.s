.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08014d78, 0x08014d78
	.set sub_0803a404, 0x0803a404
	.set sub_0803b094, 0x0803b094
	.set sub_0803b918, 0x0803b918
	.set sub_0803cfa0, 0x0803cfa0
	.global Func_0803cfd0
	.thumb_func
Func_0803cfd0:
.L_0803cfd0:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r5, #255
	ands	r5, r1
	ldr	r1, [pc, #64]
	subs	r5, #32
	lsls	r5, r5, #5
	adds	r5, r5, r1
	ldrh	r1, [r5, #0]
	adds	r6, r0, #0
	mov	r8, r1
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #6
	movs	r3, #161
	adds	r2, r2, r1
	lsls	r3, r3, #1
	adds	r6, r6, r2
	adds	r5, #2
	adds	r3, #255
	adds	r1, r6, r3
	adds	r0, r5, #0
	movs	r2, #1
	bl	sub_0803cfa0
	movs	r3, #192
	lsls	r3, r3, #1
	adds	r0, r5, #0
	adds	r1, r6, r3
	movs	r2, #15
	bl	sub_0803cfa0
	mov	r0, r8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0xa4e0
	.2byte 0x0805
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #60]
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #66
	adds	r3, r6, r2
	movs	r7, #0
	strh	r7, [r3, #0]
	mov	sl, r1
	movs	r1, #0
	sub	sp, #8
	mov	r8, r1
	bl	sub_0803b094
	movs	r3, #0
	adds	r5, r0, #0
	add	r1, sp, #4
	mov	r2, sp
	bl	sub_0803b918
	lsls	r5, r5, #1
	movs	r3, #244
	lsls	r3, r3, #4
	adds	r0, r6, r5
	adds	r5, r0, r3
	ldrh	r3, [r5, #0]
	cmp	r3, #45
	bne.n	.L_0803d086
	ldrh	r3, [r5, #2]
	movs	r0, #0
	cmp	r3, #45
	beq.n	.L_0803d06c
	b.n	.L_0803d168
.L_0803d06c:
	ldrh	r2, [r5, #4]
	cmp	r2, #45
	bne.n	.L_0803d082
	ldrh	r3, [r5, #6]
	movs	r0, #3
	eors	r3, r2
	negs	r2, r3
	orrs	r2, r3
	lsrs	r2, r2, #31
	subs	r0, r0, r2
	b.n	.L_0803d168
.L_0803d082:
	movs	r0, #1
	b.n	.L_0803d168
.L_0803d086:
	ldr	r3, [sp, #4]
	movs	r2, #192
	subs	r2, r2, r3
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r7, r2, #1
	ldrh	r1, [r5, #0]
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r2, #66
	adds	r5, r0, r2
	cmp	r1, #0
	beq.n	.L_0803d166
.L_0803d0a0:
	cmp	r1, #30
	bhi.n	.L_0803d152
	subs	r1, #3
	cmp	r1, #25
	bhi.n	.L_0803d15e
	ldr	r2, [pc, #200]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0803d146
	.4byte 0x0803d15e
	.4byte 0x0803d15e
	.4byte 0x0803d15e
	.4byte 0x0803d140
	.4byte 0x0803d11c
	.4byte 0x0803d126
	.4byte 0x0803d130
	.4byte 0x0803d13c
	.4byte 0x0803d13c
	.4byte 0x0803d15e
	.4byte 0x0803d14e
	.4byte 0x0803d14e
	.4byte 0x0803d15e
	.4byte 0x0803d13c
	.4byte 0x0803d15e
	.4byte 0x0803d15e
	.4byte 0x0803d15e
	.4byte 0x0803d15e
	.4byte 0x0803d15e
	.4byte 0x0803d15e
	.4byte 0x0803d15e
	.4byte 0x0803d15e
	.4byte 0x0803d15e
	.4byte 0x0803d15e
	.2byte 0xd14e
	.2byte 0x0803
	movs	r1, #240
	ldrh	r2, [r5, #0]
	lsls	r1, r1, #4
	adds	r1, #62
	b.n	.L_0803d138
	movs	r1, #240
	ldrh	r2, [r5, #0]
	lsls	r1, r1, #4
	adds	r1, #60
	b.n	.L_0803d138
	movs	r1, #240
	ldrh	r2, [r5, #0]
	lsls	r1, r1, #4
	adds	r1, #56
.L_0803d138:
	adds	r3, r6, r1
	strh	r2, [r3, #0]
.L_0803d13c:
	adds	r5, #2
	b.n	.L_0803d15e
	bl	sub_0803a404
	b.n	.L_0803d15e
	movs	r2, #15
	movs	r7, #0
	add	r8, r2
	b.n	.L_0803d15e
	adds	r5, #2
	b.n	.L_0803d13c
.L_0803d152:
	adds	r2, r7, #0
	mov	r0, sl
	mov	r3, r8
	bl	.L_0803cfd0
	adds	r7, r7, r0
.L_0803d15e:
	ldrh	r1, [r5, #0]
	adds	r5, #2
	cmp	r1, #0
	bne.n	.L_0803d0a0
.L_0803d166:
	movs	r0, #0
.L_0803d168:
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xd0b4
	.2byte 0x0803
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	mov	r8, r0
	mov	sl, r1
	ldr	r5, [pc, #52]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #36]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, r8
	mov	r1, sl
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c30
	bl	sub_08013164
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x00000064
	.2byte 0x857c
	.2byte 0x0803
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [r0, #0]
	movs	r2, #255
	lsrs	r3, r5, #8
	ands	r5, r2
	ldr	r2, [pc, #240]
	lsls	r3, r3, #3
	ldr	r1, [r2, r3]
	adds	r3, #4
	ldr	r2, [r2, r3]
	lsls	r3, r5, #1
	ldrh	r3, [r3, r2]
	mov	sl, r0
	adds	r1, r1, r3
	ldr	r6, [r0, #4]
	ldr	r0, [r0, #8]
	subs	r2, r1, #1
	movs	r3, #0
	movs	r5, #128
	movs	r4, #1
	mov	r9, r2
	mov	lr, r3
	movs	r7, #1
	mov	r8, r5
	b.n	.L_0803d25e
.L_0803d202:
	adds	r2, r0, #0
	movs	r3, #1
	ands	r2, r3
	asrs	r0, r0, #1
	cmp	r2, #0
	beq.n	.L_0803d25e
	cmp	r0, #0
	bne.n	.L_0803d220
	ldrb	r0, [r6, #0]
	adds	r6, #1
	adds	r2, r0, #0
	ands	r2, r3
	asrs	r0, r0, #1
	mov	r3, r8
	orrs	r0, r3
.L_0803d220:
	cmp	r2, #0
	beq.n	.L_0803d25e
	movs	r5, #1
	movs	r2, #128
	movs	r3, #0
	mov	fp, r5
	mov	ip, r2
.L_0803d22e:
	adds	r2, r4, #0
	mov	r5, fp
	ands	r2, r5
	asrs	r4, r4, #1
	cmp	r2, #0
	beq.n	.L_0803d250
	cmp	r4, #0
	bne.n	.L_0803d24c
	ldrb	r4, [r1, #0]
	adds	r1, #1
	adds	r2, r4, #0
	ands	r2, r5
	asrs	r4, r4, #1
	mov	r5, ip
	orrs	r4, r5
.L_0803d24c:
	cmp	r2, #0
	bne.n	.L_0803d254
.L_0803d250:
	adds	r3, #1
	b.n	.L_0803d25a
.L_0803d254:
	movs	r2, #1
	add	lr, r2
	subs	r3, #1
.L_0803d25a:
	cmp	r3, #0
	bge.n	.L_0803d22e
.L_0803d25e:
	adds	r2, r4, #0
	ands	r2, r7
	asrs	r4, r4, #1
	cmp	r2, #0
	beq.n	.L_0803d202
	cmp	r4, #0
	bne.n	.L_0803d27a
	ldrb	r4, [r1, #0]
	mov	r3, r8
	adds	r2, r4, #0
	asrs	r4, r4, #1
	adds	r1, #1
	ands	r2, r7
	orrs	r4, r3
.L_0803d27a:
	cmp	r2, #0
	beq.n	.L_0803d202
	mov	r5, lr
	lsls	r3, r5, #1
	adds	r1, r3, r5
	lsls	r3, r1, #2
	movs	r2, #7
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0803d2a2
	mov	r2, r9
	lsrs	r3, r1, #1
	subs	r3, r2, r3
	ldrb	r2, [r3, #0]
	subs	r3, #1
	lsls	r5, r2, #4
	ldrb	r2, [r3, #0]
	lsrs	r3, r2, #4
	orrs	r5, r3
	b.n	.L_0803d2b6
.L_0803d2a2:
	lsrs	r3, r1, #1
	mov	r5, r9
	subs	r3, r5, r3
	ldrb	r2, [r3, #0]
	subs	r3, #1
	movs	r1, #15
	ldrb	r5, [r3, #0]
	ands	r2, r1
	lsls	r2, r2, #8
	orrs	r5, r2
.L_0803d2b6:
	mov	r2, sl
	str	r0, [r2, #8]
	str	r5, [r2, #0]
	str	r6, [r2, #4]
	adds	r0, r5, #0
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08060c30
