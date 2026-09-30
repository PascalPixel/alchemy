.syntax unified
	.thumb
	.global Func_080d7d48
	.thumb_func
Func_080d7d48:
	ldr r3, .L_080d7d60
	ldr r1, .L_080d7d64
	ldr r3, [r3]
	movs r2, #1
	lsrs r3, r3, #2
	ands r3, r2
	lsls r3, r3, #2
	ldr r3, [r3, r1]
	str r3, [r0, #24]
	str r3, [r0, #28]
	bx lr
	.2byte 0x0000
.L_080d7d60:
	.4byte gFrameTick
.L_080d7d64:
	.4byte Data_080f0bfc
