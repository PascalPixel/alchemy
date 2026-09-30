.syntax unified
	.thumb
	.global Func_0803d2d0
	.thumb_func
Func_0803d2d0:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	movs r0, #152
	ldr r2, .L_0803d2ec
	lsls r0, r0, #5
	adds r0, #124
	adds r1, r3, r0
	adds r0, #2
	strh r2, [r1]
	adds r1, r3, r0
	strh r2, [r1]
	bx lr
	.2byte 0x0000
.L_0803d2ec:
	.4byte 0x000003e7
