.syntax unified
.text
	.thumb
	.global Func_080069a4
	.global Func_080069c8
	.thumb_func
Func_080069a4:
	ldr	r1, [pc, #24]
	ldrh	r0, [r1, #0]
	cmp	r0, #0
	beq .L_080069be
	ldrh	r0, [r1, #0]
	subs	r0, #1
	strh	r0, [r1, #0]
	lsls	r0, r0, #16
	cmp	r0, #0
	bne .L_080069be
	ldr	r1, [pc, #8]
	movs	r0, #1
	strb	r0, [r1, #0]
.L_080069be:
	bx	lr
	.4byte 0x02004c22
	.4byte 0x02004c24
	.thumb_func
Func_080069c8:
	adds	r2, r1, #0
	lsls	r0, r0, #24
	lsrs	r1, r0, #24
	cmp	r1, #3
	bhi .L_080069fc
	ldr	r0, [pc, #24]
	strb	r1, [r0, #0]
	ldr	r1, [pc, #24]
	ldrb	r0, [r0, #0]
	lsls	r0, r0, #2
	ldr	r3, [pc, #20]
	adds	r0, r0, r3
	str	r0, [r1, #0]
	ldr	r0, [pc, #20]
	str	r0, [r2, #0]
	movs	r0, #0
	b .L_080069fe
	.inst.n 0x0000
	.4byte 0x02004c20
	.4byte 0x02004c28
	.4byte 0x04000100
	.4byte 0x080069a5
.L_080069fc:
	movs	r0, #1
.L_080069fe:
	bx	lr
