.syntax unified
	.thumb
	.set sub_08014d00, 0x08014d00
	.set sub_08020270, 0x08020270
	.global Func_080cb8a4
	.thumb_func
Func_080cb8a4:
	push	{lr}
	movs	r1, #213
	lsls	r1, r1, #4
	movs	r0, #108
	bl	sub_08014d00
	adds	r1, r0, #0
	movs	r0, #197
	lsls	r0, r0, #1
	adds	r3, r1, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080cb8e4
	movs	r2, #140
	adds	r2, #255
	adds	r0, r1, r2
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	cmp	r3, #0
	beq.n	.L_080cb8e4
	movs	r2, #0
	strb	r2, [r0, #0]
	movs	r0, #230
	lsls	r0, r0, #1
	adds	r3, r1, r0
	ldr	r3, [r3, #0]
	adds	r3, #91
	strb	r2, [r3, #0]
	bl	sub_08020270
.L_080cb8e4:
	pop	{pc}
	.2byte 0x0000
