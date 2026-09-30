.syntax unified
	.thumb
	.global Func_081693b0
	.thumb_func
Func_081693b0:
	ldr r3, .L_081693bc
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	bx lr
.L_081693bc:
	.4byte 0x00000100
