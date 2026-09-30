.syntax unified
	.thumb
	.global Func_081269ac
	.thumb_func
Func_081269ac:
	ldr r3, .L_081269b8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	bx lr
.L_081269b8:
	.4byte 0x000000bf
