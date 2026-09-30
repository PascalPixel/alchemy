.syntax unified
	.thumb
	.global RunBattleEffect05
	.thumb_func
RunBattleEffect05:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_08099d04
	ldr r6, [r3]
	ldr r0, [r6, #16]
	movs r1, #0
	mov r10, r0
	movs r2, #0
	movs r0, #239
	movs r3, #0
	sub sp, #44
	mov r8, r1
	bl Object_Spawn
	adds r7, r0, #0
	cmp r7, #0
	bne .L_08099a1e
	b .L_08099cf0
.L_08099a1e:
	bl BattleEffect_InitializeSharedScene
	movs r0, #138
	bl AudioCommand_PlayFar
	ldr r3, [r6, #20]
	cmp r3, #0
	bne .L_08099a52
	mov r2, r10
	ldr r3, [r2, #8]
	str r3, [r6, #4]
	ldr r3, [r2, #16]
	str r3, [r6, #12]
	adds r5, r6, #0
	ldmia r5!, {r1}
	movs r0, #128
	lsls r0, r0, #13
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	ldr r1, [r5]
	ldr r2, [r6, #12]
	movs r0, #0
	bl Map_GetTerrainHeightFar
	str r0, [r6, #8]
.L_08099a52:
	mov r3, sp
	adds r3, #20
	str r3, [sp, #4]
	mov r0, r10
	ldr r1, [sp, #4]
	ldr r3, [r0, #8]
	str r3, [r1]
	movs r2, #128
	ldr r3, [r0, #12]
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r1, #4]
	ldr r3, [r0, #16]
	str r3, [r1, #8]
	add r3, sp, #8
	mov r11, r3
	ldr r3, [r6, #4]
	mov r0, r11
	str r3, [r0]
	movs r1, #128
	ldr r2, [r6, #8]
	lsls r1, r1, #14
	adds r3, r2, r1
	str r3, [r0, #4]
	ldr r3, [r6, #12]
	str r3, [r0, #8]
	adds r3, r6, #0
	adds r3, #52
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_08099a9e
	movs r0, #160
	lsls r0, r0, #15
	adds r3, r2, r0
	mov r1, r11
	str r3, [r1, #4]
.L_08099a9e:
	ldr r2, [sp, #4]
	mov r10, r11
	mov r9, r2
.L_08099aa4:
	mov r0, r10
	mov r1, r9
	ldr r5, [r1]
	ldr r3, [r0]
	subs r3, r3, r5
	mov r0, r8
	muls r0, r3
	movs r1, #10
	bl FixedPoint_Ratio
	adds r5, r5, r0
	str r5, [r7, #8]
	mov r2, r10
	mov r0, r9
	ldr r3, [r2, #4]
	ldr r5, [r0, #4]
	subs r3, r3, r5
	mov r0, r8
	muls r0, r3
	movs r1, #10
	bl FixedPoint_Ratio
	adds r5, r5, r0
	str r5, [r7, #12]
	mov r2, r9
	mov r1, r10
	ldr r5, [r2, #8]
	ldr r3, [r1, #8]
	subs r3, r3, r5
	mov r0, r8
	muls r0, r3
	movs r1, #10
	bl FixedPoint_Ratio
	movs r3, #192
	lsls r3, r3, #8
	adds r5, r5, r0
	movs r1, #10
	mov r0, r8
	muls r0, r3
	str r5, [r7, #16]
	bl FixedPoint_Ratio
	movs r3, #128
	lsls r3, r3, #7
	adds r0, r0, r3
	str r0, [r7, #24]
	str r0, [r7, #28]
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #11
	blt .L_08099aa4
	movs r0, #10
	bl WaitFrames
	adds r3, r6, #0
	adds r3, #69
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_08099bd4
	adds r3, r6, #0
	adds r3, #32
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r2, #10
	mov r9, r2
	cmp r3, #0
	bne .L_08099b3e
	movs r3, #24
	mov r9, r3
.L_08099b3e:
	movs r0, #0
	mov r8, r0
	cmp r8, r9
	bge .L_08099bcc
	mov r1, r9
	subs r1, #1
	add r6, sp, #32
	str r1, [sp, #0]
	mov r10, r6
.L_08099b50:
	ldr r3, [r7, #8]
	mov r2, r10
	str r3, [r2]
	ldr r3, [r7, #12]
	str r3, [r2, #4]
	ldr r3, [r7, #16]
	str r3, [r2, #8]
	bl Random16
	movs r3, #192
	lsls r5, r0, #2
	lsls r3, r3, #10
	adds r5, r5, r0
	adds r5, r5, r3
	bl Random16
	mov r2, r10
	adds r1, r0, #0
	adds r0, r5, #0
	bl Vector_AddPolarOffset
	ldr r0, [sp, #0]
	cmp r8, r0
	bne .L_08099b92
	movs r0, #25
	bl WaitFrames
	ldr r3, [r7, #8]
	str r3, [r6]
	ldr r3, [r7, #12]
	str r3, [r6, #4]
	ldr r3, [r7, #16]
	str r3, [r6, #8]
.L_08099b92:
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	movs r0, #240
	bl Object_Spawn
	adds r5, r0, #0
	cmp r5, #0
	beq .L_08099bb8
	ldr r3, [r6, #4]
	ldr r1, .L_08099d08
	adds r3, r3, r1
	str r3, [r5, #20]
	ldr r3, .L_08099d0c
	adds r2, r5, #0
	str r3, [r5, #108]
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
.L_08099bb8:
	movs r0, #132
	bl AudioCommand_PlayFar
	movs r0, #6
	bl WaitFrames
	movs r2, #1
	add r8, r2
	cmp r8, r9
	blt .L_08099b50
.L_08099bcc:
	movs r0, #10
	bl WaitFrames
	b .L_08099c76
.L_08099bd4:
	adds r3, r6, #0
	adds r3, #32
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r0, #10
	mov r9, r0
	cmp r3, #0
	bne .L_08099bea
	movs r1, #30
	mov r9, r1
.L_08099bea:
	mov r2, r9
	cmp r2, #0
	beq .L_08099c70
	add r6, sp, #32
	mov r8, r9
.L_08099bf4:
	ldr r3, [r7, #8]
	str r3, [r6]
	ldr r3, [r7, #12]
	str r3, [r6, #4]
	ldr r3, [r7, #16]
	str r3, [r6, #8]
	bl Random16
	movs r3, #192
	lsls r5, r0, #2
	lsls r3, r3, #10
	adds r5, r5, r0
	adds r5, r5, r3
	bl Random16
	adds r2, r6, #0
	adds r1, r0, #0
	adds r0, r5, #0
	bl Vector_AddPolarOffset
	movs r0, #142
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	lsls r0, r0, #1
	bl Object_Spawn
	adds r5, r0, #0
	cmp r5, #0
	beq .L_08099c5e
	ldr r3, .L_08099d10
	adds r2, r5, #0
	str r3, [r5, #108]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r1, [r5, #80]
	movs r0, #13
	ldrb r2, [r1, #9]
	negs r0, r0
	adds r3, r0, #0
	ands r2, r3
	movs r3, #8
	orrs r2, r3
	strb r2, [r1, #9]
	adds r0, r5, #0
	movs r1, #8
	bl Object_SetMode
	adds r0, r5, #0
	movs r1, #7
	bl Animation_ApplyChildValuesFar
.L_08099c5e:
	movs r0, #6
	bl WaitFrames
	movs r1, #1
	negs r1, r1
	add r8, r1
	mov r2, r8
	cmp r2, #0
	bne .L_08099bf4
.L_08099c70:
	movs r0, #70
	bl WaitFrames
.L_08099c76:
	movs r3, #0
	ldr r6, [sp, #4]
	mov r8, r3
	mov r10, r11
.L_08099c7e:
	mov r0, r10
	ldr r5, [r0]
	ldr r3, [r6]
	subs r3, r3, r5
	mov r0, r8
	muls r0, r3
	movs r1, #10
	bl FixedPoint_Ratio
	adds r5, r5, r0
	str r5, [r7, #8]
	mov r1, r10
	ldr r5, [r1, #4]
	ldr r3, [r6, #4]
	subs r3, r3, r5
	mov r0, r8
	muls r0, r3
	movs r1, #10
	bl FixedPoint_Ratio
	adds r5, r5, r0
	str r5, [r7, #12]
	mov r2, r10
	ldr r5, [r2, #8]
	ldr r3, [r6, #8]
	subs r3, r3, r5
	mov r0, r8
	muls r0, r3
	movs r1, #10
	bl FixedPoint_Ratio
	ldr r3, .L_08099d14
	adds r5, r5, r0
	movs r1, #10
	mov r0, r8
	muls r0, r3
	str r5, [r7, #16]
	bl FixedPoint_Ratio
	movs r3, #128
	lsls r3, r3, #9
	adds r0, r0, r3
	str r0, [r7, #24]
	str r0, [r7, #28]
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #11
	blt .L_08099c7e
	adds r0, r7, #0
	bl ObjectDispatch_ReleaseFar
	bl BattleFx_PrepareBufferInterpolation
.L_08099cf0:
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_08099d04:
	.4byte gEffectWork
.L_08099d08:
	.4byte 0xffe00000
.L_08099d0c:
	.4byte BattleFx_SpawnRandomAngleTriplet
.L_08099d10:
	.4byte BattleFx_UpdateDriftingFallObject
.L_08099d14:
	.4byte 0xffff4000
