.syntax unified
	.thumb
	.global Func_080ca18c
	.thumb_func
Func_080ca18c:
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #1
	ldr r2, .L_080ca1a0
	adds r3, r3, r1
	lsls r3, r3, #1
	adds r3, #4
	ldrh r0, [r2, r3]
	bx lr
	.2byte 0x0000
.L_080ca1a0:
	.4byte Data_080edacc
