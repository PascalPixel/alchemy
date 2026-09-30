.syntax unified
	.thumb
	.global Func_0803dc1c
	.thumb_func
Func_0803dc1c:
	push {r5, r6, r7, lr}
	movs r1, #249
	lsls r1, r1, #2
	movs r0, #72
	bl Runtime_AllocateBlock
	movs r2, #210
	adds r7, r0, #0
	lsls r2, r2, #2
	movs r5, #0
	adds r3, r7, r2
	adds r2, #4
	str r5, [r3]
	adds r3, r7, r2
	adds r2, #4
	str r5, [r3]
	adds r3, r7, r2
	adds r2, #74
	str r5, [r3]
	adds r3, r7, r2
	adds r2, #2
	strh r5, [r3]
	adds r3, r7, r2
	strh r5, [r3]
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #158
	adds r2, r7, r3
	movs r3, #128
	strh r3, [r2]
	movs r3, #232
	lsls r3, r3, #2
	adds r2, r7, r3
	movs r3, #32
	strh r3, [r2]
	movs r2, #229
	lsls r2, r2, #2
	adds r3, r7, r2
	strh r5, [r3]
	movs r3, #238
	lsls r3, r3, #2
	adds r2, r7, r3
	adds r3, #47
	strh r3, [r2]
	movs r3, #239
	lsls r3, r3, #1
	adds r2, r7, r3
	adds r3, r7, #0
	movs r1, #0
	movs r0, #0
	adds r3, #114
.L_0803dc82:
	adds r1, #1
	strh r0, [r3]
	strh r0, [r2]
	adds r3, #52
	adds r2, #52
	cmp r1, #5
	bne .L_0803dc82
	movs r2, #186
	lsls r2, r2, #1
	movs r5, #0
	adds r3, r7, r2
	adds r2, #52
	strh r5, [r3, #2]
	adds r3, r7, r2
	strh r5, [r3, #2]
	adds r3, r7, #0
	adds r3, #70
	strh r5, [r7, #10]
	strh r5, [r7, #62]
	strh r5, [r7, #18]
	strh r5, [r3]
	ldr r6, .L_0803dd20
	bl Resource_FindFreeEntry
	movs r2, #185
	lsls r2, r2, #2
	adds r3, r7, r2
	strh r0, [r3]
	movs r1, #128
	adds r2, r6, #0
	ldrh r0, [r3]
	lsls r1, r1, #1
	bl VramBlock_LoadCached
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #230
	adds r3, r7, r2
	subs r2, #4
	strh r0, [r3]
	adds r3, r7, r2
	adds r2, #24
	strh r5, [r3]
	adds r3, r7, r2
	adds r2, #28
	strh r5, [r3]
	adds r3, r7, r2
	strh r5, [r3]
	movs r3, #192
	lsls r3, r3, #2
	adds r5, r7, r3
	ldrb r3, [r5, #5]
	movs r0, #13
	negs r0, r0
	adds r2, r0, #0
	ands r2, r3
	movs r3, #17
	negs r3, r3
	ands r2, r3
	movs r3, #32
	ldrb r1, [r5, #7]
	orrs r2, r3
	movs r3, #4
	negs r3, r3
	ands r2, r3
	subs r3, #59
	movs r4, #63
	ands r3, r1
	ands r3, r4
	movs r1, #64
	orrs r3, r1
	strb r3, [r5, #7]
	ldrb r3, [r5, #9]
	ands r2, r4
	ands r0, r3
	strb r2, [r5, #5]
	strb r0, [r5, #9]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0803dd20:
	.4byte Data_0805c9c4
