.syntax unified
	.thumb
	.global Func_081693c0
	.thumb_func
Func_081693c0:
	ldr r3, .L_081693cc
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	bx lr
.L_081693cc:
	.4byte 0x00000080
