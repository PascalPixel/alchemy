.syntax unified
	.thumb
	.global UiWindow_Create
	.thumb_func
UiWindow_Create:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov lr, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	mov r12, r2
	movs r2, #161
	lsls r2, r2, #3
	adds r4, r3, r2
	ldrh r2, [r4, #22]
	movs r3, #1
	ands r3, r2
	adds r7, r1, #0
	ldr r6, [sp, #20]
	movs r5, #0
	movs r1, #0
	b .L_08039294
.L_08039286:
	adds r1, #1
	adds r4, #36
	cmp r1, #12
	beq .L_080392a2
	ldrh r2, [r4, #22]
	movs r3, #1
	ands r3, r2
.L_08039294:
	cmp r3, #0
	bne .L_08039286
	movs r2, #26
	ldrsh r3, [r4, r2]
	cmp r3, #0
	bne .L_08039286
	adds r5, r4, #0
.L_080392a2:
	cmp r5, #0
	beq .L_0803936c
	movs r3, #0
	mov r8, r3
	mov r2, r12
	mov r3, lr
	strh r7, [r5, #14]
	strh r2, [r5, #8]
	strh r3, [r5, #10]
	mov r2, r8
	mov r3, r8
	movs r7, #1
	strh r0, [r5, #12]
	strh r3, [r5, #20]
	str r2, [r5]
	str r4, [r5, #4]
	strh r7, [r5, #16]
	strh r7, [r5, #22]
	bl UiWork_ResetCounters
	movs r0, #8
	adds r3, r6, #0
	ands r3, r0
	cmp r3, #0
	beq .L_080392dc
	ldrh r3, [r5, #22]
	ldr r2, .L_08039300
	orrs r3, r2
	strh r3, [r5, #22]
.L_080392dc:
	movs r3, #32
	ands r3, r6
	cmp r3, #0
	beq .L_080392ec
	ldrh r3, [r5, #22]
	ldr r2, .L_08039304
	orrs r3, r2
	strh r3, [r5, #22]
.L_080392ec:
	movs r3, #64
	ands r3, r6
	cmp r3, #0
	beq .L_0803930c
	ldrh r3, [r5, #22]
	ldr r2, .L_08039308
	orrs r3, r2
	strh r3, [r5, #22]
	b .L_0803930c
	.2byte 0x0000
.L_08039300:
	.4byte 0x00000008
.L_08039304:
	.4byte 0x00000020
.L_08039308:
	.4byte 0x00000040
.L_0803930c:
	movs r3, #128
	ands r3, r6
	cmp r3, #0
	beq .L_0803931c
	ldrh r3, [r5, #22]
	ldr r2, .L_08039348
	orrs r3, r2
	strh r3, [r5, #22]
.L_0803931c:
	movs r1, #128
	lsls r1, r1, #1
	adds r3, r6, #0
	ands r3, r1
	cmp r3, #0
	beq .L_08039330
	ldrh r2, [r5, #22]
	adds r3, r1, #0
	orrs r3, r2
	strh r3, [r5, #22]
.L_08039330:
	movs r3, #2
	ands r3, r6
	cmp r3, #0
	beq .L_0803935a
	ldrh r3, [r5, #22]
	ldr r2, .L_0803934c
	strh r7, [r5, #26]
	orrs r3, r2
	mov r2, r8
	strh r3, [r5, #22]
	b .L_08039350
	.2byte 0x0000
.L_08039348:
	.4byte 0x00000080
.L_0803934c:
	.4byte 0x00000002
.L_08039350:
	strh r2, [r5, #24]
	adds r0, r5, #0
	bl UiWork_DrawByAttributes
	b .L_0803936c
.L_0803935a:
	movs r3, #7
	strh r0, [r5, #26]
	strh r3, [r5, #24]
	adds r0, r5, #0
	bl UiWork_WaitUntilField1aClear
	movs r0, #1
	bl WaitFrames
.L_0803936c:
	adds r0, r5, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
