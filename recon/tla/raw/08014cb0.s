.syntax unified
	.thumb
	.global Func_08014cb0
	.thumb_func
Func_08014cb0:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3]
	movs r0, #129
	lsls r0, r0, #18
	subs r0, r0, r3
	bx lr
	.2byte 0x0000
