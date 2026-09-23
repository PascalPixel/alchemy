.syntax unified
	.thumb
	.set sub_08013300, 0x08013300
	.set sub_080149f8, 0x080149f8
	.set sub_08014a24, 0x08014a24
	.global Func_08014ae0
	.thumb_func
Func_08014ae0:
.L_08014ae0:
	push	{r5, lr}
	ldr	r3, [pc, #60]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08014b1c
	ldr	r4, [pc, #56]
	ldrb	r3, [r0, #0]
	ldr	r2, [r4, #0]
	movs	r1, #0
	adds	r0, #1
	cmp	r3, #0
	beq.n	.L_08014b1a
	ldr	r4, [pc, #44]
	movs	r5, #240
	lsls	r5, r5, #8
.L_08014afe:
	orrs	r3, r5
	strh	r3, [r2, #0]
	adds	r2, #2
	cmp	r2, r4
	bne.n	.L_08014b0a
	ldr	r2, [pc, #32]
.L_08014b0a:
	adds	r1, #1
	cmp	r1, #31
	bhi.n	.L_08014b18
	ldrb	r3, [r0, #0]
	adds	r0, #1
	cmp	r3, #0
	bne.n	.L_08014afe
.L_08014b18:
	ldr	r4, [pc, #8]
.L_08014b1a:
	str	r2, [r4, #0]
.L_08014b1c:
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x03001110
	.4byte 0x030011c4
	.4byte 0x06002500
	.2byte 0x2000
	.2byte 0x0600
	push	{r5, lr}
	adds	r5, r1, #0
	subs	r3, r5, #1
	cmp	r3, #7
	bls.n	.L_08014b3c
	movs	r5, #8
.L_08014b3c:
	bl	sub_080149f8
	ldr	r0, [pc, #8]
	subs	r0, r0, r5
	bl	.L_08014ae0
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x1258
	.2byte 0x0300
	push	{r5, lr}
	adds	r5, r1, #0
	subs	r3, r5, #1
	cmp	r3, #9
	bls.n	.L_08014b5c
	movs	r5, #10
.L_08014b5c:
	bl	sub_08014a24
	ldr	r0, [pc, #8]
	subs	r0, r0, r5
	bl	.L_08014ae0
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x125a
	.2byte 0x0300
	ldr	r3, [pc, #40]
	sub	sp, #4
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r1, [pc, #32]
	ldr	r2, [pc, #32]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #20]
	str	r3, [r2, #0]
	movs	r2, #128
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #3
	adds	r3, #8
	strh	r2, [r3, #0]
	add	sp, #4
	bx	lr
	.4byte 0xf000f000
	.4byte 0x06002000
	.4byte 0x85000140
	.2byte 0x11c4
	.2byte 0x0300
	push	{lr}
	ldr	r0, [pc, #132]
	bl	sub_08013300
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #192
	adds	r3, #212
	lsls	r1, r1, #19
	ldr	r2, [pc, #120]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	lsls	r2, r2, #24
	ldr	r0, [pc, #112]
	ldr	r1, [pc, #116]
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #0
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	movs	r2, #131
	ldr	r3, [pc, #100]
	lsls	r2, r2, #7
	strh	r2, [r3, #0]
	movs	r2, #228
	lsls	r2, r2, #6
	adds	r2, #96
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r2, #197
	lsls	r2, r2, #6
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r2, #164
	lsls	r2, r2, #6
	adds	r2, #32
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r2, #146
	lsls	r2, r2, #7
	adds	r2, #160
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r2, #162
	lsls	r2, r2, #7
	adds	r2, #192
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r2, #178
	lsls	r2, r2, #7
	adds	r2, #224
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #32]
	adds	r1, #32
	adds	r2, #224
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	pop	{pc}
	movs	r0, r0
	.4byte 0x00000013
	.4byte 0x84000800
	.4byte 0x08017af0
	.4byte 0x050001e0
	.4byte 0x050001e8
	.2byte 0x7b10
	.2byte 0x0801
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #12]
	ldr	r1, [pc, #12]
	adds	r2, #224
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	movs	r0, r0
	.4byte 0x08017b10
	.2byte 0x0200
	.2byte 0x0500
	sub	sp, #4
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	movs	r4, #192
	movs	r3, #128
	movs	r2, #133
	lsls	r4, r4, #18
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, r4, #0
	adds	r2, #64
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #12]
	add	sp, #4
	str	r3, [r4, #4]
	ldr	r3, [pc, #8]
	str	r3, [r4, #0]
	bx	lr
	movs	r0, r0
	.4byte 0x03001300
	.2byte 0x0000
	.2byte 0x0203
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #4]
	ldr	r0, [pc, #4]
	subs	r0, r0, r3
	bx	lr
	.2byte 0x6fc0
	.2byte 0x0300
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #0]
	movs	r0, #129
	lsls	r0, r0, #18
	subs	r0, r0, r3
	bx	lr
	.2byte 0x0000
