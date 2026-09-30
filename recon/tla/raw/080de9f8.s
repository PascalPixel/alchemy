.syntax unified
	.thumb
	.global Func_080de9f8
	.thumb_func
Func_080de9f8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r7, #0
	mov r10, r3
	ldr r6, [r3, #16]
	bl BattleEffect_InitializeSharedScene
	movs r2, #0
	mov r8, r2
.L_080dea18:
	ldr r2, [r6, #12]
	movs r3, #128
	lsls r3, r3, #14
	movs r0, #148
	adds r2, r2, r3
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	lsls r0, r0, #1
	bl Object_Spawn
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080dea5e
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r5, #28]
	str r3, [r5, #24]
	ldr r3, .L_080deb9c
	adds r2, r5, #0
	str r3, [r5, #108]
	adds r2, #100
	movs r3, #120
	strh r3, [r2]
	lsls r3, r7, #13
	adds r2, #2
	strh r3, [r2]
	subs r2, #17
	movs r3, #4
	strb r3, [r2]
	mov r1, r8
	ldr r0, [r5, #80]
	bl Func_080dc0d8
	mov r8, r0
.L_080dea5e:
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #7
	ble .L_080dea18
	mov r2, r8
	ldrb r2, [r2, #16]
	movs r0, #130
	mov r9, r2
	bl Audio_PlayCue
	movs r0, #110
	bl WaitFrames
	movs r0, #148
	lsls r0, r0, #1
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Object_Spawn
	adds r6, r0, #0
	adds r5, r6, #0
	cmp r6, #0
	beq .L_080deac0
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r6, #28]
	str r3, [r6, #24]
	mov r2, r10
	ldr r3, [r2, #4]
	movs r1, #7
	str r3, [r6, #8]
	ldr r3, [r2, #8]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r6, #12]
	mov r2, r10
	ldr r3, [r2, #12]
	adds r2, r6, #0
	str r3, [r6, #16]
	adds r2, #85
	movs r3, #4
	strb r3, [r2]
	bl Animation_ApplyChildValuesFar
.L_080deac0:
	movs r0, #131
	bl Audio_PlayCue
	movs r0, #12
	bl WaitFrames
	cmp r6, #0
	beq .L_080deafe
	movs r3, #3
	movs r7, #0
	mov r8, r3
.L_080dead6:
	adds r3, r7, #0
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_080deaea
	adds r0, r5, #0
	movs r1, #9
	bl Animation_ApplyChildValuesFar
	b .L_080deaf2
.L_080deaea:
	adds r0, r5, #0
	movs r1, #10
	bl Animation_ApplyChildValuesFar
.L_080deaf2:
	movs r0, #2
	adds r7, #1
	bl WaitFrames
	cmp r7, #29
	ble .L_080dead6
.L_080deafe:
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValuesFar
	movs r0, #84
	bl Audio_PlayCue
	cmp r5, #0
	beq .L_080deb38
	ldr r3, .L_080deba0
	adds r2, r6, #0
	str r3, [r6, #108]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	mov r3, r10
	adds r3, #32
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080deb32
	movs r0, #128
	bl WaitFrames
	b .L_080deb38
.L_080deb32:
	movs r0, #192
	bl WaitFrames
.L_080deb38:
	cmp r6, #0
	beq .L_080deb76
	movs r3, #255
	adds r2, r6, #0
	lsls r3, r3, #8
	adds r2, #100
	adds r3, #255
	strh r3, [r2]
	movs r3, #160
	lsls r3, r3, #11
	str r3, [r6, #48]
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r6, #52]
	subs r2, #10
	movs r3, #0
	strb r3, [r2]
	movs r1, #192
	movs r2, #232
	lsls r1, r1, #16
	lsls r2, r2, #8
	adds r0, r6, #0
	bl Motion_SetTargetPositionFromMagnitudeAngle
	adds r0, r6, #0
	bl Object_CommitPosition
	adds r0, r6, #0
	bl Func_080200c8
.L_080deb76:
	mov r3, r9
	cmp r3, #96
	beq .L_080deb82
	mov r0, r9
	bl Resource_ResetEntry
.L_080deb82:
	mov r2, r10
	ldr r3, [r2, #36]
	cmp r3, #0
	beq .L_080deb8e
	mov lr, r3
	.2byte 0xf800
.L_080deb8e:
	bl BattleFx_PrepareBufferInterpolation
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_080deb9c:
	.4byte Func_080de864
.L_080deba0:
	.4byte Func_080de8d0
