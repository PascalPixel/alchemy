.syntax unified
	.thumb
	.global Func_080d82e0
	.thumb_func
Func_080d82e0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_080d8468
	adds r5, r0, #0
	movs r0, #133
	lsls r0, r0, #2
	adds r7, r3, r0
	ldr r0, [r7]
	sub sp, #12
	bl Object_GetById
	mov r10, r0
	adds r0, r5, #0
	bl Object_GetById
	adds r6, r0, #0
	cmp r6, #0
	bne .L_080d830c
	b .L_080d85a2
.L_080d830c:
	bl BattleFx_InitializeSlots
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	ldr r0, .L_080d846c
	mov r9, r3
	bl Unnamed_080b0840Far
	movs r0, #30
	bl WaitFrames
	adds r2, r6, #0
	movs r3, #0
	adds r2, #91
	strb r3, [r2]
	movs r0, #152
	bl Audio_PlayCue
	adds r0, r5, #0
	movs r1, #4
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #152
	bl Audio_PlayCue
	movs r1, #4
	movs r2, #15
	adds r0, r5, #0
	bl ObjectMotion_Launch
	movs r0, #30
	bl WaitFrames
	ldr r3, .L_080d8470
	movs r0, #153
	str r3, [r6, #108]
	bl Audio_PlayCue
	adds r0, r5, #0
	movs r1, #8
	movs r2, #22
	bl ObjectMotion_Launch
	movs r0, #140
	bl Audio_PlayCue
	movs r1, #166
	lsls r1, r1, #9
	movs r5, #128
	adds r1, #204
	lsls r5, r5, #9
	adds r2, r5, #0
	adds r0, r1, #0
	bl Func_08020228
	ldr r3, .L_080d8474
	adds r0, r6, #0
	str r3, [r6, #108]
	movs r1, #3
	bl Object_SetMode
	movs r0, #90
	bl WaitFrames
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	ldr r0, [r7]
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl WaitFrames
	ldr r0, [r7]
	bl Object_GetById
	movs r1, #28
	bl Object_SetMode
	movs r0, #30
	bl WaitFrames
	ldr r1, .L_080d8478
	adds r2, r5, #0
	adds r0, r1, #0
	bl Func_08020228
	ldr r3, [r6, #8]
	mov r8, sp
	str r3, [sp, #0]
	mov r0, r8
	ldr r3, [r6, #12]
	mov r5, r9
	str r3, [sp, #4]
	adds r5, #80
	ldr r3, [r6, #16]
	movs r7, #23
	str r3, [sp, #8]
	bl Camera_WorldToScreen
	mov r6, r8
.L_080d83dc:
	movs r1, #168
	ldr r2, [r6]
	ldr r3, [r6, #8]
	adds r0, r5, #0
	lsls r1, r1, #2
	bl Func_080ebec8
	adds r0, r5, #0
	ldr r1, .L_080d847c
	bl Func_080ebeb4
	adds r0, r5, #0
	movs r1, #7
	bl Func_080ebea8
	ldr r0, [r5]
	movs r1, #11
	bl Animation_ApplyChildValuesToRecordFar
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #40]
	bl Random16
	movs r2, #192
	lsls r2, r2, #9
	adds r0, r0, r2
	str r0, [r5, #44]
	subs r7, #1
	movs r0, #1
	bl WaitFrames
	adds r5, #72
	cmp r7, #0
	bge .L_080d83dc
	movs r0, #140
	bl WaitFrames
	mov r2, r9
	movs r1, #2
	adds r2, #144
	movs r7, #23
.L_080d8430:
	movs r3, #5
	ldrsb r3, [r2, r3]
	cmp r3, #0
	beq .L_080d843a
	strb r1, [r2]
.L_080d843a:
	subs r7, #1
	adds r2, #72
	cmp r7, #0
	bge .L_080d8430
	movs r0, #20
	bl WaitFrames
	movs r0, #1
	movs r1, #1
	movs r2, #1
	bl Func_08020228
	movs r0, #30
	bl WaitFrames
	ldr r3, .L_080d8464
	movs r7, #0
	mov r5, r8
	mov r9, r3
	b .L_080d8480
	.2byte 0x0000
.L_080d8464:
	.4byte 0x00000000
.L_080d8468:
	.4byte gPartyState
.L_080d846c:
	.4byte 0x00201204
.L_080d8470:
	.4byte BattleFx_AdvanceSpinAngle
.L_080d8474:
	.4byte BattleFx_ShrinkObjectAndDestroyFast
.L_080d8478:
	.4byte 0x00019999
.L_080d847c:
	.4byte Func_080d81ec
.L_080d8480:
	mov r0, r10
	ldr r1, [r0, #8]
	movs r3, #240
	str r1, [r5]
	lsls r3, r3, #15
	ldr r2, [r0, #12]
	adds r2, r2, r3
	str r2, [r5, #4]
	ldr r3, [r0, #16]
	movs r0, #168
	str r3, [r5, #8]
	lsls r0, r0, #2
	bl Object_Spawn
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080d84e8
	bl Random16
	movs r1, #3
	bl __udivsi3
	movs r2, #128
	lsls r2, r2, #9
	adds r0, r0, r2
	adds r2, r6, #0
	adds r2, #100
	movs r3, #100
	str r0, [r6, #28]
	str r0, [r6, #24]
	movs r1, #24
	strh r3, [r2]
	lsls r0, r7, #16
	bl __divsi3
	adds r3, r6, #0
	adds r3, #102
	strh r0, [r3]
	ldr r3, .L_080d85b0
	mov r0, r9
	str r3, [r6, #108]
	adds r3, r6, #0
	adds r3, #85
	strb r0, [r3]
	movs r1, #7
	adds r0, r6, #0
	bl Object_SetMode
	adds r0, r6, #0
	movs r1, #11
	bl Animation_ApplyChildValuesFar
.L_080d84e8:
	adds r7, #1
	cmp r7, #23
	ble .L_080d8480
	movs r0, #100
	bl WaitFrames
	movs r0, #149
	lsls r0, r0, #1
	bl Audio_PlayCue
	movs r0, #1
	bl WaitFrames
	movs r0, #151
	bl Audio_PlayCue
	mov r2, r10
	ldr r3, [r2, #8]
	mov r0, r8
	str r3, [r0]
	movs r7, #0
	ldr r3, [r2, #12]
	movs r2, #144
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r0, #4]
	mov r0, r10
	ldr r3, [r0, #16]
	mov r2, r8
	mov r5, r8
	str r3, [r2, #8]
	b .L_080d857c
.L_080d8528:
	movs r3, #153
	lsls r3, r3, #8
	adds r3, #153
	adds r2, r6, #0
	adds r2, #85
	str r3, [r6, #28]
	str r3, [r6, #24]
	movs r3, #2
	strb r3, [r2]
	movs r3, #160
	lsls r3, r3, #11
	str r3, [r6, #40]
	ldr r3, [r6, #12]
	adds r7, #1
	str r3, [r6, #20]
	bl Random16
	movs r3, #179
	lsls r3, r3, #9
	adds r3, #102
	adds r0, r0, r3
	str r0, [r6, #48]
	bl Random16
	movs r1, #128
	adds r2, r0, #0
	lsls r1, r1, #14
	adds r0, r6, #0
	bl Motion_SetTargetPositionFromMagnitudeAngle
	adds r0, r6, #0
	movs r1, #11
	bl Animation_ApplyChildValuesFar
	adds r2, r6, #0
	adds r2, #94
	movs r3, #8
	strh r3, [r2]
	adds r0, r6, #0
	ldr r1, .L_080d85b4
	bl ObjectDispatch_InitializeFar
.L_080d857c:
	cmp r7, #7
	bgt .L_080d8594
	movs r0, #168
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	lsls r0, r0, #2
	bl Object_Spawn
	adds r6, r0, #0
	cmp r6, #0
	bne .L_080d8528
.L_080d8594:
	movs r0, #15
	bl WaitFrames
	bl Func_08108060
	bl Func_080d7ab4
.L_080d85a2:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d85b0:
	.4byte Func_080d8174
.L_080d85b4:
	.4byte BattleFx_CommonParticleScript
