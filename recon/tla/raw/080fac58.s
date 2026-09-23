.syntax unified
	.thumb
	.set sub_08014644, 0x08014644
	.set sub_08020030, 0x08020030
	.global Func_080fac58
	.thumb_func
Func_080fac58:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	movs	r1, #139
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080fac94
	movs	r5, #0
.L_080fac72:
	asrs	r5, r5, #24
	lsls	r3, r5, #2
	adds	r3, #248
	ldr	r0, [r6, r3]
	movs	r1, #1
	bl	sub_08020030
	movs	r1, #139
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r3, r6, r1
	adds	r5, #1
	ldrb	r3, [r3, #0]
	lsls	r5, r5, #24
	asrs	r2, r5, #24
	cmp	r2, r3
	blt.n	.L_080fac72
.L_080fac94:
	ldr	r0, [pc, #4]
	bl	sub_08014644
	pop	{r5, r6, pc}
	.2byte 0xabe1
	.2byte 0x080f
	push	{lr}
	cmp	r0, #196
	bgt.n	.L_080facae
	cmp	r0, #193
	blt.n	.L_080facae
	movs	r0, #1
	b.n	.L_080facb0
.L_080facae:
	movs	r0, #0
.L_080facb0:
	pop	{pc}
	.align 2, 0
