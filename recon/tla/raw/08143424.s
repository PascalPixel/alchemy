.syntax unified
	.thumb
	.global Func_08143424
	.thumb_func
Func_08143424:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #92]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #216
	adds r3, r2, r0
	ldrh r3, [r3]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #32
	strh r3, [r1]
	adds r0, #2
	adds r3, r2, r0
	ldrh r3, [r3]
	adds r1, #6
	strh r3, [r1]
	subs r0, #10
	adds r3, r2, r0
	ldr r3, [r3]
	adds r1, #2
	str r3, [r1]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #212
	adds r2, r2, r3
	ldr r3, [r2]
	adds r1, #4
	str r3, [r1]
	bx lr
