.syntax unified
	.thumb
	.global RunBattleEffect13
	.thumb_func
RunBattleEffect13:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0809aec0
	ldr r5, [r3]
	ldr r1, [r5, #20]
	ldr r7, [r5, #16]
	sub sp, #40
	str r1, [sp, #0]
	ldr r3, [r7, #8]
	add r2, sp, #16
	str r3, [r2]
	ldr r3, [r7, #12]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r2, #4]
	ldr r3, [r7, #16]
	str r3, [r2, #8]
	adds r3, r5, #0
	adds r3, #32
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r11, r2
	cmp r3, #0
	beq .L_0809aec4
	ldr r3, [r7, #8]
	add r2, sp, #4
	str r3, [r2]
	ldr r3, [r7, #12]
	movs r0, #128
	lsls r0, r0, #14
	adds r3, r3, r0
	str r3, [r2, #4]
	ldr r3, [r7, #16]
	str r3, [r2, #8]
	ldr r1, [r5]
	mov r9, r2
	bl Vector_AddPolarOffset
	b .L_0809aedc
.L_0809aec0:
	.4byte gEffectWork
.L_0809aec4:
	add r3, sp, #4
	mov r9, r3
	ldr r3, [r5, #4]
	mov r1, r9
	str r3, [r1]
	movs r2, #128
	ldr r3, [r5, #8]
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r1, #4]
	ldr r3, [r5, #12]
	str r3, [r1, #8]
.L_0809aedc:
	ldr r1, [r5, #4]
	add r0, sp, #28
	str r1, [r0]
	movs r3, #128
	ldr r2, [r5, #8]
	lsls r3, r3, #14
	adds r2, r2, r3
	str r2, [r0, #4]
	ldr r3, [r5, #12]
	str r3, [r0, #8]
	movs r0, #215
	bl Object_Spawn
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0809aefe
	b .L_0809b092
.L_0809aefe:
	bl BattleEffect_InitializeSharedScene
	movs r0, #138
	bl AudioCommand_PlayFar
	ldrh r3, [r7, #6]
	strh r3, [r6, #6]
	ldr r3, .L_0809af34
	ldr r2, .L_0809af30
	str r3, [r6, #48]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	adds r0, r6, #0
	movs r1, #5
	bl Object_SetMode
	adds r0, r6, #0
	movs r1, #1
	bl Animation_ApplyChildValuesFar
	movs r7, #0
	mov r10, r11
	mov r8, r9
	b .L_0809af38
.L_0809af30:
	.4byte 0x00000000
.L_0809af34:
	.4byte 0x00014ccc
.L_0809af38:
	mov r2, r10
	mov r1, r8
	ldr r5, [r2]
	ldr r3, [r1]
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	movs r1, #10
	bl FixedPoint_Ratio
	adds r5, r5, r0
	str r5, [r6, #8]
	mov r2, r10
	mov r1, r8
	ldr r5, [r2, #4]
	ldr r3, [r1, #4]
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	movs r1, #10
	bl FixedPoint_Ratio
	adds r5, r5, r0
	str r5, [r6, #12]
	mov r2, r10
	mov r1, r8
	ldr r5, [r2, #8]
	ldr r3, [r1, #8]
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	movs r1, #10
	bl FixedPoint_Ratio
	movs r3, #192
	lsls r3, r3, #8
	adds r5, r5, r0
	movs r1, #10
	adds r0, r7, #0
	muls r0, r3
	str r5, [r6, #16]
	bl FixedPoint_Ratio
	movs r3, #128
	lsls r3, r3, #7
	adds r0, r0, r3
	str r0, [r6, #24]
	str r0, [r6, #28]
	adds r7, #1
	movs r0, #1
	bl WaitFrames
	cmp r7, #11
	blt .L_0809af38
	movs r0, #10
	bl WaitFrames
	adds r0, r6, #0
	movs r1, #6
	bl Object_SetMode
	movs r0, #15
	bl WaitFrames
	movs r5, #9
.L_0809afba:
	ldr r3, [r6, #12]
	ldr r1, .L_0809b0a4
	adds r3, r3, r1
	str r3, [r6, #12]
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0809afba
	adds r0, r6, #0
	movs r1, #5
	bl Object_SetMode
	movs r0, #132
	bl AudioCommand_PlayFar
	ldr r2, [sp, #0]
	cmp r2, #0
	beq .L_0809afee
	ldr r3, .L_0809b0a8
	ldr r2, [r2, #12]
	ldr r0, [sp, #0]
	adds r1, r3, #0
	bl Object_SetPositionAndResetMotionFar
.L_0809afee:
	movs r0, #20
	bl WaitFrames
	movs r5, #12
.L_0809aff6:
	ldr r3, [r6, #12]
	movs r1, #192
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r6, #12]
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0809aff6
	movs r0, #10
	bl WaitFrames
	movs r0, #114
	bl AudioCommand_PlayFar
	movs r7, #0
	mov r10, r9
	mov r8, r11
.L_0809b01e:
	mov r2, r8
	mov r1, r10
	ldr r3, [r2]
	ldr r5, [r1]
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	movs r1, #10
	bl FixedPoint_Ratio
	adds r5, r5, r0
	str r5, [r6, #8]
	mov r2, r8
	mov r1, r10
	ldr r3, [r2, #4]
	ldr r5, [r1, #4]
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	movs r1, #10
	bl FixedPoint_Ratio
	adds r5, r5, r0
	str r5, [r6, #12]
	mov r2, r8
	mov r1, r10
	ldr r3, [r2, #8]
	ldr r5, [r1, #8]
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	movs r1, #10
	bl FixedPoint_Ratio
	ldr r3, .L_0809b0ac
	adds r5, r5, r0
	movs r1, #10
	adds r0, r7, #0
	muls r0, r3
	str r5, [r6, #16]
	bl FixedPoint_Ratio
	movs r2, #128
	lsls r2, r2, #9
	adds r0, r0, r2
	str r0, [r6, #24]
	str r0, [r6, #28]
	adds r7, #1
	movs r0, #1
	bl WaitFrames
	cmp r7, #11
	blt .L_0809b01e
	adds r0, r6, #0
	bl ObjectDispatch_ReleaseFar
	bl BattleFx_PrepareBufferInterpolation
.L_0809b092:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_0809b0a4:
	.4byte 0xfffe0000
.L_0809b0a8:
	.4byte 0xfff70000
.L_0809b0ac:
	.4byte 0xffff4000
