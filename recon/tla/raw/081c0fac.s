.syntax unified
	.thumb
	.global Func_081c0fac
	.thumb_func
Func_081c0fac:
	ldr r3, .L_081c0fb8
	strh r0, [r3]
	ldr r3, .L_081c0fbc
	strh r1, [r3]
	bx lr
	.2byte 0x0000
.L_081c0fb8:
	.4byte gMusicVolumeTarget
.L_081c0fbc:
	.4byte Data_02005810
