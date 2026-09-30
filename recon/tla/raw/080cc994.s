.syntax unified
	.thumb
	.global Func_080cc994
	.thumb_func
Func_080cc994:
	ldr r3, [r0]
	ldrb r1, [r3]
	adds r3, #1
	str r3, [r0]
	ldrb r2, [r3]
	adds r3, #1
	lsls r2, r2, #8
	orrs r1, r2
	str r3, [r0]
	adds r0, r1, #0
	bx lr
	.2byte 0x0000
