.syntax unified
	.thumb
	.set sub_08014274, 0x08014274
	.set sub_08014644, 0x08014644
	.global Func_0810be3c
	.thumb_func
Func_0810be3c:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r0, [pc, #36]
	ldr	r6, [r3, #0]
	bl	sub_08014644
	movs	r5, #192
	lsls	r5, r5, #4
	adds	r5, #200
	movs	r7, #14
.L_0810be54:
	ldrh	r3, [r5, r6]
	cmp	r3, #96
	beq.n	.L_0810be60
	adds	r0, r3, #0
	bl	sub_08014274
.L_0810be60:
	subs	r7, #1
	adds	r5, #2
	cmp	r7, #0
	bge.n	.L_0810be54
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xbdb1
	.2byte 0x0810
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r4, [r3, #0]
	lsls	r3, r0, #1
	adds	r3, r3, r0
	movs	r0, #192
	lsls	r0, r0, #4
	adds	r0, #24
	lsls	r3, r3, #2
	adds	r3, r3, r0
	adds	r4, r4, r3
	ldr	r3, [pc, #16]
	ldrh	r0, [r4, #2]
	ands	r1, r3
	ldr	r3, [pc, #16]
	strb	r2, [r4, #0]
	ands	r3, r0
	orrs	r3, r1
	strh	r3, [r4, #2]
	b.n	.L_0810bea4
	movs	r0, r0
	.4byte 0x000001ff
	.2byte 0xfe00
	.2byte 0xffff
.L_0810bea4:
	bx	lr
	.2byte 0x0000
