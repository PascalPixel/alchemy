.syntax unified
	.thumb
	.global Func_081433b4
	.thumb_func
Func_081433b4:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #92]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #188
	adds r3, r2, r0
	ldrh r3, [r3]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #64
	strh r3, [r1]
	adds r0, #2
	adds r3, r2, r0
	ldrh r3, [r3]
	adds r1, #4
	strh r3, [r1]
	adds r0, #2
	adds r3, r2, r0
	ldrh r3, [r3]
	subs r1, #2
	strh r3, [r1]
	adds r0, #2
	adds r3, r2, r0
	ldrh r3, [r3]
	adds r1, #4
	strh r3, [r1]
	adds r0, #2
	adds r3, r2, r0
	ldrh r3, [r3]
	adds r1, #2
	strh r3, [r1]
	adds r0, #2
	adds r3, r2, r0
	ldrh r3, [r3]
	adds r1, #2
	strh r3, [r1]
	adds r0, #2
	adds r3, r2, r0
	ldrh r3, [r3]
	subs r1, #74
	strh r3, [r1]
	adds r0, #2
	adds r3, r2, r0
	ldrh r3, [r3]
	adds r1, #80
	strh r3, [r1]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #204
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r1, #2
	strh r3, [r1]
	bx lr
	.2byte 0x0000
