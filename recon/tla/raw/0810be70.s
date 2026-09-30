.syntax unified
	.thumb
	.global Func_0810be70
	.thumb_func
Func_0810be70:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r4, [r3]
	lsls r3, r0, #1
	adds r3, r3, r0
	movs r0, #192
	lsls r0, r0, #4
	adds r0, #24
	lsls r3, r3, #2
	adds r3, r3, r0
	adds r4, r4, r3
	ldr r3, .L_0810be9c
	ldrh r0, [r4, #2]
	ands r1, r3
	ldr r3, .L_0810bea0
	strb r2, [r4]
	ands r3, r0
	orrs r3, r1
	strh r3, [r4, #2]
	b .L_0810bea4
	.2byte 0x0000
.L_0810be9c:
	.4byte 0x000001ff
.L_0810bea0:
	.4byte 0xfffffe00
.L_0810bea4:
	bx lr
	.2byte 0x0000
