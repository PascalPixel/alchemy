.syntax unified
	.thumb
	.global Func_08043230
	.thumb_func
Func_08043230:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #204
	ldr	r3, [r3, #0]
	movs	r1, #192
	lsls	r1, r1, #6
	adds	r1, #60
	adds	r2, r3, r1
	movs	r0, #0
.L_08043244:
	ldrb	r3, [r2, #28]
	cmp	r3, #0
	beq.n	.L_08043258
	adds	r0, #1
	adds	r2, #64
	cmp	r0, #2
	bls.n	.L_08043244
	movs	r0, #186
	lsls	r0, r0, #2
	adds	r0, #255
.L_08043258:
	pop	{pc}
	.2byte 0x0000
