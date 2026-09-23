.syntax unified
	.thumb
	.set sub_081b34a8, 0x081b34a8
	.set sub_081c0010, 0x081c0010
	.global Func_081b2000
	.thumb_func
Func_081b2000:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x2009
	.2byte 0x081b
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	ldr	r2, [pc, #28]
	ldr	r3, [r3, #4]
	movs	r0, #227
	lsls	r0, r0, #1
	str	r3, [r2, #0]
	adds	r0, #255
	bl	sub_081c0010
	bl	sub_081b34a8
	movs	r0, #0
	b.n	.L_081b2038
	.4byte 0x00000040
	.4byte 0x02000240
	.2byte 0x11bc
	.2byte 0x0300
.L_081b2038:
	pop	{pc}
	.align 2, 0
