.syntax unified
	.thumb
	push	{lr}
	movs	r0, #169
	lsls	r0, r0, #1
	bl	GameFlag_SetBitFar
	movs	r0, #179
	lsls	r0, r0, #1
	bl	GameFlag_SetBitFar
	bl	0x080202d8
	bl	0x080146d4
	pop	{pc}
	movs	r3, #192
	lsls	r3, r3, #18
	sub	sp, #4
	adds	r3, #220
	ldr	r4, [r3, #0]
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r1, r4, #0
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #255
	strb	r3, [r4, #28]
	adds	r2, r4, #0
	movs	r3, #1
	strb	r3, [r4, #30]
	strb	r3, [r4, #31]
	adds	r2, #246
	adds	r4, #247
	strb	r3, [r2, #0]
	add	sp, #4
	strb	r3, [r4, #0]
	bx	lr
	movs	r0, r0
	.2byte 0x033b
	.2byte 0x8500
	.global Func_080f811c
	.thumb_func
Func_080f811c:
.L_080f811c:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	ldr	r0, [r6, #0]
	sub	sp, #4
	adds	r5, r3, #0
	ldr	r4, [sp, #20]
	cmp	r0, #0
	beq.n	.L_080f8142
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_080f813a
	movs	r0, #0
	b.n	.L_080f8158
.L_080f813a:
	bl	0x08038260
	movs	r0, #0
	b.n	.L_080f8158
.L_080f8142:
	movs	r3, #255
	ands	r4, r3
	adds	r0, r1, #0
	ldr	r3, [sp, #16]
	adds	r1, r2, #0
	adds	r2, r5, #0
	str	r4, [sp, #0]
	bl	0x08038010
	str	r0, [r6, #0]
	movs	r0, #1
.L_080f8158:
	add	sp, #4
	pop	{r5, r6, pc}
