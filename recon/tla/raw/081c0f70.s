.syntax unified
	.thumb
	.balign 4
	.global Func_081c0f70
	.thumb_func
Func_081c0f70:
	ldr	r3, [pc, #8]
	strh	r0, [r3, #0]
	ldr	r3, [pc, #8]
	strh	r1, [r3, #0]
	bx	lr
	movs	r0, r0
	.4byte 0x02005834
	.4byte 0x0200580c