.syntax unified
	.thumb
	.set sub_080cdac0, 0x080cdac0
	.global Func_080e8cfc
	.thumb_func
Func_080e8cfc:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r2, r3, #0
	adds	r2, #224
	ldr	r4, [r2, #0]
	ldr	r2, [r3, #108]
	ldr	r6, [r3, #92]
	sub	sp, #8
	movs	r3, #0
	adds	r2, #20
	str	r3, [r6, #80]
	str	r2, [sp, #4]
	movs	r7, #0
.L_080e8d18:
	ldr	r3, [sp, #4]
	ldmia	r3!, {r5}
	adds	r2, r3, #0
	str	r2, [sp, #4]
	cmp	r5, #0
	beq.n	.L_080e8d74
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080e8d74
	ldr	r3, [r4, #16]
	cmp	r5, r3
	beq.n	.L_080e8d74
	ldrh	r0, [r4, #30]
	ldr	r3, [pc, #32]
	movs	r2, #24
	ldrsh	r1, [r4, r2]
	orrs	r0, r3
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	adds	r2, r7, #0
	str	r4, [sp, #0]
	bl	sub_080cdac0
	movs	r3, #1
	negs	r3, r3
	ldr	r4, [sp, #0]
	cmp	r0, r3
	beq.n	.L_080e8d74
	b.n	.L_080e8d58
	movs	r0, r0
	.2byte 0x0100
	.2byte 0x0000
.L_080e8d58:
	ldr	r2, [r6, #80]
	lsls	r3, r2, #2
	adds	r3, #32
	str	r5, [r6, r3]
	ldr	r3, [r5, #80]
	lsls	r2, r2, #1
	ldrh	r3, [r3, #18]
	adds	r2, #64
	strh	r3, [r6, r2]
	ldr	r3, [r6, #80]
	adds	r3, #1
	str	r3, [r6, #80]
	cmp	r3, #8
	beq.n	.L_080e8d7a
.L_080e8d74:
	adds	r7, #1
	cmp	r7, #79
	ble.n	.L_080e8d18
.L_080e8d7a:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
