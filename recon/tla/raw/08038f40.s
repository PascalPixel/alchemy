.syntax unified
	.thumb
	.set sub_080145a8, 0x080145a8
	.set sub_08014d00, 0x08014d00
	.set sub_0803a448, 0x0803a448
	.set sub_0803a4b0, 0x0803a4b0
	.set sub_08041abc, 0x08041abc
	.global Func_08038f40
	.thumb_func
Func_08038f40:
	push	{r5, r6, lr}
	movs	r6, #152
	lsls	r6, r6, #5
	adds	r6, #144
	adds	r1, r6, #0
	movs	r0, #60
	bl	sub_08014d00
	adds	r1, r6, #0
	ldr	r3, [pc, #68]
	adds	r5, r0, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2301
	strb	r3, [r5, #3]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #70
	adds	r2, r5, r3
	movs	r3, #99
	strh	r3, [r2, #0]
	movs	r3, #15
	strb	r3, [r5, #7]
	movs	r1, #160
	adds	r5, #8
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #40]
	lsls	r1, r1, #3
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf7ff
	.2byte 0xffc3
	.2byte 0xf004
	.2byte 0xf9a5
	.2byte 0x21c8
	lsls	r1, r1, #4
	ldr	r0, [pc, #24]
	bl	sub_080145a8
	bl	sub_0803a448
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x03000258
	.4byte 0x03000260
	.4byte 0xf000f000
	.2byte 0x90b9
	.2byte 0x0803
.L_08038fa8:
	push	{lr}
	mov	ip, r3
	mov	r3, r9
	push	{r3}
	mov	r3, ip
	adds	r3, r0, #0
	movs	r0, #192
	mov	r2, r9
	sub	sp, #4
	lsls	r0, r0, #2
	str	r2, [sp, #0]
	adds	r4, r1, #0
	adds	r0, #255
	movs	r2, #192
	ands	r4, r0
	lsls	r2, r2, #19
	ands	r0, r3
	adds	r2, #16
	lsls	r0, r0, #5
	lsls	r4, r4, #5
	adds	r0, r0, r2
	subs	r2, #16
	adds	r1, r4, r2
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #192
	lsls	r3, r3, #19
	adds	r3, #12
	adds	r4, r4, r3
	adds	r0, r4, #0
	ldr	r3, [pc, #12]
	movs	r1, #20
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xb001
	pop	{r3}
	mov	r9, r3
	pop	{pc}
	.2byte 0x0258
	.2byte 0x0300
	push	{r5, r6, lr}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	movs	r6, #152
	lsls	r6, r6, #5
	adds	r6, #144
	mov	r8, r0
	adds	r1, r6, #0
	movs	r0, #60
	bl	sub_08014d00
	ldr	r3, [pc, #136]
	adds	r1, r6, #0
	adds	r5, r0, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2398
	lsls	r3, r3, #5
	adds	r3, #70
	adds	r2, r5, r3
	movs	r1, #1
	movs	r3, #99
	strb	r1, [r5, #3]
	adds	r0, r5, #0
	strh	r3, [r2, #0]
	strb	r1, [r5, #5]
	movs	r3, #15
	movs	r1, #160
	ldr	r2, [pc, #108]
	strb	r3, [r5, #7]
	lsls	r1, r1, #3
	ldr	r3, [pc, #104]
	adds	r0, #8
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf7ff
	.2byte 0xff5c
	.2byte 0x21c8
	lsls	r1, r1, #4
	ldr	r0, [pc, #92]
	bl	sub_080145a8
	mov	r0, r8
	bl	sub_0803a4b0
	movs	r0, #240
	lsls	r0, r0, #8
	mov	r9, sp
	movs	r1, #128
	adds	r0, #19
	bl	.L_08038fa8
	movs	r0, #240
	lsls	r0, r0, #8
	mov	r9, sp
	movs	r1, #129
	adds	r0, #20
	bl	.L_08038fa8
	movs	r0, #240
	lsls	r0, r0, #8
	movs	r1, #130
	adds	r0, #21
	mov	r9, sp
	bl	.L_08038fa8
	movs	r1, #224
	lsls	r1, r1, #4
	adds	r1, #58
	movs	r2, #4
	movs	r3, #2
	adds	r5, r5, r1
.L_08039096:
	subs	r3, #1
	strb	r2, [r5, #0]
	subs	r5, #1
	cmp	r3, #0
	bge.n	.L_08039096
	pop	{r3, r5}
	mov	r8, r3
	mov	r9, r5
	pop	{r5, r6, pc}
	.4byte 0x03000258
	.4byte 0xf000f000
	.4byte 0x03000260
	.2byte 0x90b9
	.2byte 0x0803
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #60]
	ldrb	r4, [r7, #3]
	cmp	r4, #0
	beq.n	.L_08039116
	ldrb	r3, [r7, #6]
	cmp	r3, #0
	bne.n	.L_08039116
	ldr	r2, [pc, #72]
	movs	r3, #1
	adds	r5, r7, #0
	ands	r3, r4
	adds	r5, #8
	mov	ip, r2
	cmp	r3, #0
	beq.n	.L_080390de
	movs	r4, #63
.L_080390de:
	movs	r3, #63
	ands	r4, r3
	movs	r6, #128
	movs	r3, #1
	lsrs	r4, r4, #1
	mov	lr, r3
	lsls	r6, r6, #1
.L_080390ec:
	adds	r3, r4, #0
	mov	r2, lr
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803910a
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r5, #0
	mov	r1, ip
	adds	r2, #64
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_0803910a:
	lsrs	r4, r4, #1
	adds	r5, r5, r6
	add	ip, r6
	cmp	r4, #0
	bne.n	.L_080390ec
	strb	r4, [r7, #3]
.L_08039116:
	pop	{r5, r6, r7, pc}
	.2byte 0x2000
	.2byte 0x0600
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	adds	r6, r2, #0
	mov	sl, r3
	lsls	r3, r1, #6
	add	r3, sl
	lsls	r2, r0, #1
	adds	r3, r3, r2
	adds	r5, r3, #0
	movs	r4, #240
	adds	r3, r1, r7
	sub	sp, #4
	mov	r8, r1
	adds	r5, #8
	lsls	r4, r4, #8
	cmp	r3, #20
	bls.n	.L_0803914e
	movs	r3, #20
	subs	r7, r3, r1
.L_0803914e:
	cmp	r6, #1
	bhi.n	.L_08039154
	movs	r6, #2
.L_08039154:
	cmp	r6, #30
	bls.n	.L_0803915a
	movs	r6, #30
.L_0803915a:
	cmp	r7, #1
	bhi.n	.L_08039160
	movs	r7, #2
.L_08039160:
	cmp	r7, #30
	bls.n	.L_08039166
	movs	r7, #30
.L_08039166:
	adds	r2, r6, #0
	mov	r1, r8
	adds	r3, r7, #0
	str	r4, [sp, #0]
	bl	sub_08041abc
	movs	r2, #0
	ldr	r4, [sp, #0]
	cmp	r2, r7
	bcs.n	.L_080391b2
	movs	r3, #32
	subs	r3, r3, r6
	lsls	r1, r3, #1
.L_08039180:
	mov	r0, sl
	ldrb	r3, [r0, #5]
	cmp	r3, #0
	beq.n	.L_0803919a
	mov	r0, r8
	movs	r4, #240
	lsls	r4, r4, #8
	adds	r3, r0, r2
	adds	r4, #127
	cmp	r3, #16
	bhi.n	.L_0803919a
	movs	r4, #240
	lsls	r4, r4, #8
.L_0803919a:
	movs	r3, #0
	cmp	r3, r6
	bcs.n	.L_080391aa
.L_080391a0:
	adds	r3, #1
	strh	r4, [r5, #0]
	adds	r5, #2
	cmp	r3, r6
	bcc.n	.L_080391a0
.L_080391aa:
	adds	r2, #1
	adds	r5, r5, r1
	cmp	r2, r7
	bcc.n	.L_08039180
.L_080391b2:
	movs	r3, #1
	mov	r2, sl
	strb	r3, [r2, #3]
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.align 2, 0
