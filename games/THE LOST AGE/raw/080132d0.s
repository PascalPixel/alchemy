.syntax unified
	.thumb
	.set sub_08013300, 0x08013300
	.global Overlay_080132d0
Overlay_080132d0:
	push	{lr}
	ldr	r0, [pc, #32]
	bl	sub_08013300
	ldr	r4, [pc, #28]
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, r4, #0
	adds	r2, #3
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #0
	str	r3, [r4, #12]
	pop	{pc}
	movs	r0, r0
	.4byte 0x00000002
	.2byte 0x7804
	.2byte 0x0300
	bx	lr
