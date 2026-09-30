.syntax unified
	.thumb
	.global Func_08014e1c
	.thumb_func
Func_08014e1c:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, .L_08014e30
	ldr r3, [r3, #8]
	str r3, [r2]
	ldr r2, .L_08014e34
	movs r3, #0
	str r3, [r2]
	bx lr
	.2byte 0x0000
.L_08014e30:
	.4byte Data_03001220
.L_08014e34:
	.4byte Data_030011cc
