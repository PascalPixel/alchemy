.syntax unified
	.thumb
	.global Func_080eb2d8
	.thumb_func
Func_080eb2d8:
	movs r4, #192
	lsls r4, r4, #18
	adds r3, r4, #0
	adds r3, #180
	ldr r3, [r3]
	lsls r1, r1, #6
	adds r1, r1, r0
	lsls r1, r1, #1
	ldr r4, [r4, #96]
	ldrh r3, [r1, r3]
	strb r2, [r4, r3]
	bx lr
