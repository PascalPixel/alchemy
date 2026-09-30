.syntax unified
	.thumb
	.global BattlePresentation_RunUnitTransition
	.thumb_func
BattlePresentation_RunUnitTransition:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #124
	add r7, sp, #12
	str r1, [sp, #8]
	adds r1, r7, #0
	mov r9, r0
	ldr r5, .L_080b9f14
	bl BattlePres_BuildTargetList
	mov r0, r9
	ldrb r0, [r0]
	str r0, [sp, #4]
	mov r1, r9
	ldrb r1, [r1, #2]
	str r1, [sp, #0]
	mov r2, r9
	ldr r3, [r2, #88]
	movs r2, #128
	lsls r2, r2, #8
	ands r3, r2
	cmp r3, #0
	beq .L_080b9f18
	adds r3, r5, #0
	adds r3, #140
	ldr r2, [r3]
	movs r3, #128
	lsls r3, r3, #6
	cmp r0, #7
	bls .L_080b9f0a
	movs r3, #160
	lsls r3, r3, #7
.L_080b9f0a:
	str r3, [r2]
	movs r3, #60
	str r3, [r2, #4]
	b .L_080b9f32
	.2byte 0x0000
.L_080b9f14:
	.4byte gBattleWork
.L_080b9f18:
	adds r3, r5, #0
	adds r3, #140
	ldr r1, [r3]
	ldr r3, [sp, #4]
	ldr r2, .L_080b9f78
	cmp r3, #7
	bhi .L_080b9f2a
	movs r2, #128
	lsls r2, r2, #6
.L_080b9f2a:
	ldr r3, [r1]
	cmp r3, r2
	beq .L_080b9f32
	str r2, [r1]
.L_080b9f32:
	movs r1, #0
	movs r0, #0
	bl BattlePres_SetActorModes
	ldr r3, .L_080b9f7c
	ldr r3, [r3]
	adds r3, #65
	ldrb r0, [r3]
	movs r3, #2
	negs r3, r3
	ands r0, r3
	bl UiWindow_DrawPartyStatusContentsFar
	ldr r0, [sp, #4]
	bl GetBattleObjectSlot
	ldr r0, [r0]
	mov r11, r0
	add r0, sp, #96
	ldr r2, .L_080b9f80
	ldr r3, .L_080b9f74
	mov r10, r0
	strh r3, [r2]
	movs r0, #3
	mov r1, r10
	bl BattleParty_ListActorIds
	movs r6, #0
	mov r8, r0
	cmp r0, #0
	beq .L_080b9fc2
	movs r5, #0
	b .L_080b9f84
.L_080b9f74:
	.4byte 0x00003f40
.L_080b9f78:
	.4byte 0xffffe000
.L_080b9f7c:
	.4byte gBattleWork
.L_080b9f80:
	.4byte 0x04000050
.L_080b9f84:
	mov r1, r10
	ldrh r3, [r5, r1]
	cmp r3, #254
	beq .L_080b9fba
	ldr r2, [sp, #4]
	adds r0, r3, #0
	cmp r0, r2
	beq .L_080b9fb2
	ldr r3, [sp, #0]
	movs r2, #0
	cmp r3, #7
	bhi .L_080b9f9e
	movs r2, #1
.L_080b9f9e:
	movs r3, #0
	cmp r0, #7
	bhi .L_080b9fa6
	movs r3, #1
.L_080b9fa6:
	cmp r2, r3
	beq .L_080b9fba
	movs r1, #1
	bl BattlePres_SetActorRecordMode
	b .L_080b9fba
.L_080b9fb2:
	mov r0, r11
	movs r1, #3
	bl Object_SetMode
.L_080b9fba:
	adds r6, #1
	adds r5, #2
	cmp r6, r8
	bne .L_080b9f84
.L_080b9fc2:
	movs r0, #154
	bl AudioCommand_PlayFar
	mov r2, r9
	ldr r1, [r2, #80]
	movs r3, #0
	ldr r0, [r7, #8]
	movs r2, #0
	bl BattleFx_PlayUnitElementEffect
	ldr r0, [sp, #8]
	movs r3, #1
	ands r3, r0
	cmp r3, #0
	beq .L_080b9fe8
	ldr r0, [sp, #4]
	movs r1, #1
	bl BattlePres_SetActorRecordMode
.L_080b9fe8:
	ldr r1, .L_080ba014
	ldr r5, .L_080ba018
	movs r6, #0
	mov r11, r1
.L_080b9ff0:
	mov r2, r11
	subs r3, r2, r6
	ldr r0, .L_080ba01c
	orrs r3, r5
	strh r3, [r0]
	adds r6, #1
	movs r0, #1
	bl WaitFrames
	cmp r6, #16
	bne .L_080b9ff0
	mov r1, r9
	ldr r3, [r1, #92]
	cmp r3, #0
	beq .L_080ba044
	cmp r3, #1
	bne .L_080ba032
	b .L_080ba020
.L_080ba014:
	.4byte 0x00000010
.L_080ba018:
	.4byte 0x00001000
.L_080ba01c:
	.4byte 0x04000052
.L_080ba020:
	ldrb r1, [r1]
	movs r0, #0
	bl BattleEv_Push
	ldr r1, .L_080ba074
	movs r0, #4
	bl BattleEv_Push
	b .L_080ba03a
.L_080ba032:
	ldr r1, .L_080ba078
	movs r0, #4
	bl BattleEv_Push
.L_080ba03a:
	bl BattleEv_DispatchQueued
	bl BattlePres_RunWithZeroArguments
	b .L_080ba1ba
.L_080ba044:
	movs r6, #0
	movs r2, #0
	cmp r6, r8
	bcs .L_080ba09e
	ldr r0, [sp, #8]
	movs r3, #1
	ands r0, r3
	mov r12, r0
	mov r5, r10
	mov r1, r10
.L_080ba058:
	ldrh r3, [r5]
	ldr r0, [sp, #4]
	adds r5, #2
	cmp r3, r0
	bne .L_080ba07c
	mov r3, r12
	cmp r3, #0
	bne .L_080ba098
	add r0, sp, #4
	ldrh r0, [r0]
	adds r2, #1
	strh r0, [r1]
	b .L_080ba096
	.2byte 0x0000
.L_080ba074:
	.4byte 0x00000856
.L_080ba078:
	.4byte 0x00000855
.L_080ba07c:
	ldr r0, [sp, #0]
	movs r4, #0
	cmp r0, #7
	bls .L_080ba086
	movs r4, #1
.L_080ba086:
	movs r0, #0
	cmp r3, #7
	bhi .L_080ba08e
	movs r0, #1
.L_080ba08e:
	cmp r4, r0
	beq .L_080ba098
	strh r3, [r1]
	adds r2, #1
.L_080ba096:
	adds r1, #2
.L_080ba098:
	adds r6, #1
	cmp r6, r8
	bcc .L_080ba058
.L_080ba09e:
	ldr r3, .L_080ba0c4
	lsls r2, r2, #1
	mov r1, r10
	strh r3, [r1, r2]
	mov r0, r10
	movs r1, #0
	bl BattleActor_SpawnObjectsForList
	mov r2, r9
	movs r3, #1
	ldrsb r3, [r2, r3]
	movs r0, #0
	cmp r0, r3
	bge .L_080ba0d8
	mov r12, r3
	mov r1, r10
	adds r2, #2
	mov r0, r12
	b .L_080ba0c8
.L_080ba0c4:
	.4byte 0x000000ff
.L_080ba0c8:
	ldrb r3, [r2]
	subs r0, #1
	strh r3, [r1]
	adds r2, #1
	adds r1, #2
	cmp r0, #0
	bne .L_080ba0c8
	mov r0, r12
.L_080ba0d8:
	ldr r2, .L_080ba0f0
	lsls r3, r0, #1
	mov r0, r10
	strh r2, [r0, r3]
	ldr r3, [r7, #20]
	movs r6, #0
	adds r2, r7, #0
	cmp r3, #0
	beq .L_080ba138
	movs r5, #0
	b .L_080ba0f4
	.2byte 0x0000
.L_080ba0f0:
	.4byte 0x000000ff
.L_080ba0f4:
	lsls r3, r6, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	bl GetBattleObjectSlot
	movs r1, #0
	ldr r0, [r0]
	bl GetMotionRecord
	adds r3, r0, #0
	adds r3, #39
	ldrb r3, [r3]
	subs r3, #1
	movs r1, #0
	cmp r3, #0
	beq .L_080ba12c
	mov r12, r3
	adds r3, r5, r7
	adds r2, r3, #0
	adds r2, #52
	adds r0, #40
.L_080ba11e:
	ldmia r0!, {r3}
	ldrb r3, [r3, #5]
	adds r1, #1
	strb r3, [r2]
	adds r2, #1
	cmp r1, r12
	bne .L_080ba11e
.L_080ba12c:
	ldr r3, [r7, #20]
	adds r6, #1
	adds r5, #4
	adds r2, r7, #0
	cmp r6, r3
	bne .L_080ba0f4
.L_080ba138:
	mov r3, r9
	ldr r2, [r3, #88]
	movs r3, #128
	lsls r3, r3, #8
	ands r2, r3
	cmp r2, #0
	beq .L_080ba152
	ldr r0, [sp, #0]
	cmp r0, #7
	bls .L_080ba15a
	movs r3, #0
	str r3, [r7, #4]
	b .L_080ba162
.L_080ba152:
	mov r1, r9
	ldrb r3, [r1, #2]
	cmp r3, #7
	bhi .L_080ba160
.L_080ba15a:
	movs r3, #1
	str r3, [r7, #4]
	b .L_080ba162
.L_080ba160:
	str r2, [r7, #4]
.L_080ba162:
	mov r2, r9
	ldr r3, [r2, #88]
	movs r2, #128
	lsls r2, r2, #10
	ands r3, r2
	cmp r3, #0
	beq .L_080ba178
	ldr r3, [r7, #4]
	movs r2, #1
	eors r3, r2
	str r3, [r7, #4]
.L_080ba178:
	movs r1, #200
	ldr r0, .L_080ba1ac
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	mov r3, r9
	ldr r0, [r3, #88]
	movs r3, #128
	lsls r3, r3, #8
	ands r3, r0
	cmp r3, #0
	beq .L_080ba198
	adds r0, r7, #0
	bl BattleFx_InitializeModeFar
	b .L_080ba1b6
.L_080ba198:
	movs r3, #128
	lsls r3, r3, #7
	ands r3, r0
	cmp r3, #0
	beq .L_080ba1b0
	adds r0, r7, #0
	bl BattleFx_DispatchByIdRangeFar
	b .L_080ba1b6
	.2byte 0x0000
.L_080ba1ac:
	.4byte BattleEvent_Playback
.L_080ba1b0:
	adds r0, r7, #0
	bl BattleFx_DispatchModeFar
.L_080ba1b6:
	bl BattleEventRuntime_WaitForReady
.L_080ba1ba:
	bl BattleActor_CommitPlacement
	movs r0, #3
	mov r1, r10
	bl BattleParty_ListActorIds
	ldr r2, .L_080ba1f8
	ldr r3, .L_080ba1f4
	mov r8, r0
	strh r3, [r2]
	movs r6, #0
	cmp r0, #0
	beq .L_080ba216
	movs r5, #0
.L_080ba1d6:
	mov r0, r10
	ldrh r3, [r5, r0]
	cmp r3, #254
	beq .L_080ba20e
	ldr r1, [sp, #4]
	adds r0, r3, #0
	cmp r0, r1
	beq .L_080ba20e
	ldr r3, [sp, #0]
	movs r2, #0
	cmp r3, #7
	bhi .L_080ba1fc
	movs r2, #1
	b .L_080ba1fc
	.2byte 0x0000
.L_080ba1f4:
	.4byte 0x00003f40
.L_080ba1f8:
	.4byte 0x04000050
.L_080ba1fc:
	movs r3, #0
	cmp r0, #7
	bhi .L_080ba204
	movs r3, #1
.L_080ba204:
	cmp r2, r3
	beq .L_080ba20e
	movs r1, #1
	bl BattlePres_SetActorRecordMode
.L_080ba20e:
	adds r6, #1
	adds r5, #2
	cmp r6, r8
	bne .L_080ba1d6
.L_080ba216:
	ldr r7, .L_080ba240
	ldr r5, .L_080ba23c
	movs r6, #0
.L_080ba21c:
	adds r3, r6, #0
	orrs r3, r5
	strh r3, [r7]
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #16
	bne .L_080ba21c
	mov r0, r8
	movs r6, #0
	cmp r0, #0
	beq .L_080ba254
	mov r5, r10
	b .L_080ba244
	.2byte 0x0000
.L_080ba23c:
	.4byte 0x00001000
.L_080ba240:
	.4byte 0x04000052
.L_080ba244:
	ldrh r0, [r5]
	movs r1, #0
	adds r6, #1
	adds r5, #2
	bl BattlePres_SetActorRecordMode
	cmp r6, r8
	bne .L_080ba244
.L_080ba254:
	movs r1, #0
	movs r2, #0
	movs r3, #100
	movs r0, #0
	bl BattlePres_SetupTransitionScene
	movs r0, #1
	bl WaitFrames
	movs r0, #0
	add sp, #124
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
