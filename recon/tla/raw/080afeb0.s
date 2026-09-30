.syntax unified
	.thumb
	.balign 4
	.global Func_080afeb0
	.thumb_func
Func_080afeb0:
	push	{lr}
	ldr	r1, [pc, #24]
	ldr	r2, [pc, #24]
	ldr	r3, [r1, #16]
	adds	r3, r3, r0
	cmp	r3, r2
	ble.n	.L_080afec0
	adds	r3, r2, #0
.L_080afec0:
	cmp	r3, #0
	bge.n	.L_080afec6
	movs	r3, #0
.L_080afec6:
	str	r3, [r1, #16]
	adds	r0, r3, #0
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x423f
	.2byte 0x000f