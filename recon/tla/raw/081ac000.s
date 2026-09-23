.syntax unified
	.thumb
	.set sub_081ac15c, 0x081ac15c
	.set sub_081c0010, 0x081c0010
	.global Func_081ac000
	.thumb_func
Func_081ac000:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xc009
	.2byte 0x081a
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #2
	bl	sub_081c0010
	bl	sub_081ac15c
	movs	r0, #0
	b.n	.L_081ac024
	.2byte 0x0040
	.2byte 0x0000
.L_081ac024:
	pop	{pc}
	.2byte 0x0000
