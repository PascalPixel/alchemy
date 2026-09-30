.syntax unified
	.thumb
	.global Func_081ac008
	.thumb_func
Func_081ac008:
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #2
	bl	Audio_PlayCue
	bl	0x081ac15c
	movs	r0, #0
	b.n	.L_081ac024
	.2byte 0x0040
	.2byte 0x0000
.L_081ac024:
	pop	{pc}
	.2byte 0x0000
