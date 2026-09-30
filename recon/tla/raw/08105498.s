.syntax unified
	.thumb
	.global Func_08105498
	.thumb_func
Func_08105498:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r4, [r3]
	lsls r3, r0, #1
	adds r3, r3, r0
	movs r0, #189
	lsls r0, r0, #4
	lsls r3, r3, #2
	adds r3, r3, r0
	adds r4, r4, r3
	ldr r3, .L_081054c0
	ldrh r0, [r4, #2]
	ands r1, r3
	ldr r3, .L_081054c4
	strb r2, [r4]
	ands r3, r0
	orrs r3, r1
	strh r3, [r4, #2]
	b .L_081054c8
.L_081054c0:
	.4byte 0x000001ff
.L_081054c4:
	.4byte 0xfffffe00
.L_081054c8:
	bx lr
	.2byte 0x0000
