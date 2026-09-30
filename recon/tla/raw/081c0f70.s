.syntax unified
	.thumb
	.global Func_081c0f70
	.thumb_func
Func_081c0f70:
	ldr r3, .L_081c0f7c
	strh r0, [r3]
	ldr r3, .L_081c0f80
	strh r1, [r3]
	bx lr
	.2byte 0x0000
.L_081c0f7c:
	.4byte Data_02005834
.L_081c0f80:
	.4byte Data_0200580c
