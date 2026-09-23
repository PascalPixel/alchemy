.syntax unified
	.thumb
	.global Func_08016d7c
	.thumb_func
Func_08016d7c:
	push	{lr}
	ldr	r1, [pc, #20]
	lsls	r3, r0, #20
	lsrs	r0, r3, #23
	ldrb	r2, [r1, r0]
	adds	r3, r2, #0
	cmp	r3, #254
	bhi.n	.L_08016d90
	adds	r3, r2, #1
	strb	r3, [r1, r0]
.L_08016d90:
	ldrb	r0, [r1, r0]
	pop	{pc}
	.2byte 0x0040
	.2byte 0x0200
	push	{lr}
	ldr	r1, [pc, #20]
	lsls	r3, r0, #20
	lsrs	r0, r3, #23
	ldrb	r2, [r1, r0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_08016dac
	adds	r3, #255
	strb	r3, [r1, r0]
.L_08016dac:
	ldrb	r0, [r1, r0]
	pop	{pc}
	.4byte 0x02000040
