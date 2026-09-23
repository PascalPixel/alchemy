.syntax unified
	.thumb
	.set sub_0811a5fc, 0x0811a5fc
	.set sub_0811b9e0, 0x0811b9e0
	.global Func_0811bc2c
	.thumb_func
Func_0811bc2c:
	push	{r5, r6, lr}
	cmp	r0, #0
	beq.n	.L_0811bc62
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	movs	r2, #15
	ands	r2, r3
	cmp	r2, #1
	beq.n	.L_0811bc46
	cmp	r2, #2
	beq.n	.L_0811bc4e
	b.n	.L_0811bc62
.L_0811bc46:
	ldr	r0, [r0, #80]
	bl	sub_0811b9e0
	b.n	.L_0811bc62
.L_0811bc4e:
	ldr	r5, [r0, #80]
	movs	r6, #3
.L_0811bc52:
	ldmia	r5!, {r0}
	cmp	r0, #0
	beq.n	.L_0811bc5c
	bl	sub_0811b9e0
.L_0811bc5c:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L_0811bc52
.L_0811bc62:
	pop	{r5, r6, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_0811a5fc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #36]
	adds	r0, r2, #0
	adds	r0, #132
	cmp	r5, #7
	ble.n	.L_0811bc7c
	subs	r5, #120
.L_0811bc7c:
	adds	r5, #116
	ldrb	r3, [r2, r5]
	movs	r1, #0
	cmp	r3, #255
	beq.n	.L_0811bc8e
	ldrb	r3, [r2, r5]
	movs	r2, #44
	muls	r3, r2
	adds	r1, r0, r3
.L_0811bc8e:
	movs	r3, #1
	strh	r3, [r1, #40]
	movs	r0, #0
	pop	{r5, pc}
	.align 2, 0
