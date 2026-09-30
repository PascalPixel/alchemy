.syntax unified
	.thumb
	.global Func_08041f90
	.thumb_func
Func_08041f90:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	movs r2, #240
	lsls r2, r2, #4
	adds r2, #60
	adds r3, r3, r2
	strh r0, [r3]
	bx lr
	.2byte 0x0000
