.syntax unified
	.thumb
	.global Func_081c0fd0
	.thumb_func
Func_081c0fd0:
	ldr r3, .L_081c0fd8
	ldrb r0, [r3]
	bx lr
	.2byte 0x0000
.L_081c0fd8:
	.4byte gMusicRestoreDelay
