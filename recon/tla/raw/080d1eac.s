.syntax unified
	.thumb
	.set sub_080d1e60, 0x080d1e60
	.set sub_080d1e84, 0x080d1e84
	.global Func_080d1eac
	.thumb_func
Func_080d1eac:
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #42
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080d1ec8
	bl	sub_080d1e60
	ldrb	r0, [r0, #2]
	cmp	r0, #255
	bne.n	.L_080d1ecc
.L_080d1ec8:
	movs	r0, #0
	b.n	.L_080d1ed2
.L_080d1ecc:
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r0, r0, r3
.L_080d1ed2:
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl	sub_080d1e84
	bl	sub_080d1e60
	ldrb	r0, [r0, #3]
	pop	{pc}
	.2byte 0x0000
