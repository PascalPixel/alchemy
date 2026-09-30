.syntax unified
	.thumb
	.global Func_0811bcec
	.thumb_func
Func_0811bcec:
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #36]
	movs r3, #44
	adds r2, r0, #0
	muls r2, r3
	adds r3, r2, #0
	adds r3, #144
	ldr r3, [r4, r3]
	adds r2, #148
	str r3, [r1]
	movs r3, #0
	str r3, [r1, #4]
	movs r0, #0
	ldr r3, [r4, r2]
	str r3, [r1, #8]
	bx lr
	.2byte 0x0000
