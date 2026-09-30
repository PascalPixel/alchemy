.syntax unified
	.thumb
	.balign 4
	.global Func_081c0fac
	.thumb_func
Func_081c0fac:
	ldr	r3, [pc, #8]
	strh	r0, [r3, #0]
	ldr	r3, [pc, #8]
	strh	r1, [r3, #0]
	bx	lr
	movs	r0, r0
	.4byte 0x02005838
	.2byte 0x5810
	.2byte 0x0200
	.global Func_081c0fc0
	.thumb_func
Func_081c0fc0:
	push	{lr}
	bl	Audio_StopAllPlayers
	pop	{pc}
	.global Func_081c0fc8
	.thumb_func
Func_081c0fc8:
	push	{lr}
	bl	Audio_ResumeAllPlayers
	pop	{pc}
	.global Func_081c0fd0
	.thumb_func
Func_081c0fd0:
	ldr	r3, [pc, #4]
	ldrb	r0, [r3, #0]
	bx	lr
	movs	r0, r0
	.2byte 0x5800
	.2byte 0x0200