.syntax unified
	.thumb
	.set sub_080a17c4, 0x080a17c4
	.global Overlay_080a22f4
Overlay_080a22f4:
	ldr	r3, [pc, #20]
	ldr	r0, [pc, #24]
	ldr	r1, [pc, #24]
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r1, #28
	ldr	r0, [pc, #24]
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	.4byte 0x040000d4
	.4byte 0x05000200
	.4byte 0x050001c0
	.4byte 0x80000010
	.4byte 0x050001e8
	.2byte 0x0001
	.2byte 0x8000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r9, r3
	ldr	r3, [pc, #136]
	ldr	r3, [r3, #0]
	sub	sp, #4
	mov	r8, r3
	movs	r2, #13
	adds	r3, #72
	movs	r6, #31
.L_080a233e:
	ldmia	r3!, {r5}
	cmp	r5, #0
	beq.n	.L_080a2346
	strb	r2, [r5, #5]
.L_080a2346:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L_080a233e
	adds	r6, r1, #0
	adds	r0, r0, r6
	cmp	r6, r0
	bge.n	.L_080a23ac
	lsls	r2, r6, #2
	adds	r3, r2, #0
	adds	r3, #72
	mov	r1, r8
	ldr	r5, [r1, r3]
	cmp	r5, #0
	beq.n	.L_080a23ac
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r8
	ldrb	r3, [r3, #0]
	subs	r3, #1
	cmp	r6, r3
	bgt.n	.L_080a23ac
	adds	r3, r2, r1
	adds	r2, r3, #0
	ldr	r7, [sp, #32]
	mov	sl, r0
	adds	r2, #72
.L_080a237a:
	mov	r3, r9
	strh	r3, [r5, #6]
	strh	r7, [r5, #8]
	adds	r0, r5, #0
	str	r2, [sp, #0]
	bl	sub_080a17c4
	adds	r6, #1
	movs	r3, #1
	strb	r3, [r5, #5]
	adds	r7, #16
	ldr	r2, [sp, #0]
	cmp	r6, sl
	bge.n	.L_080a23ac
	adds	r2, #4
	ldr	r5, [r2, #0]
	cmp	r5, #0
	beq.n	.L_080a23ac
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r8
	ldrb	r3, [r3, #0]
	subs	r3, #1
	cmp	r6, r3
	ble.n	.L_080a237a
.L_080a23ac:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
