.syntax unified
	.thumb
	.global FunctionHead_08097c3c
	.thumb_func
FunctionHead_08097c3c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_08097f50
	ldr r3, [r3]
	sub sp, #52
	str r3, [sp, #24]
	ldr r0, [r3, #16]
	str r0, [sp, #20]
	movs r7, #128
	ldr r6, [r3, #20]
	ldr r3, [r3]
	lsls r7, r7, #8
	adds r3, r3, r7
	movs r1, #0
	str r3, [sp, #8]
	str r1, [sp, #4]
	cmp r6, #0
	bne .L_08097c6c
	b .L_08097f3c
.L_08097c6c:
	bl BattleEffect_InitializeSharedScene
	ldr r2, [sp, #20]
	str r6, [r2, #104]
	ldr r0, [sp, #20]
	ldr r1, .L_08097f54
	bl Engine_ObjectSetScript
	ldr r0, [sp, #20]
	bl BattleFx_StartItemBreak
	mov r10, r0
	cmp r0, #0
	bne .L_08097c8e
	bl BattleFx_PrepareBufferInterpolation
	b .L_08097f3c
.L_08097c8e:
	mov r3, r10
	str r6, [r3, #104]
	movs r0, #40
	ldr r3, [r6, #8]
	add r0, sp
	str r3, [r0]
	movs r5, #128
	ldr r3, [r6, #12]
	lsls r5, r5, #13
	adds r3, r3, r5
	str r3, [r0, #4]
	ldr r3, [r6, #16]
	mov r9, r0
	str r3, [r0, #8]
	ldr r1, [sp, #8]
	adds r0, r5, #0
	mov r2, r9
	bl Vector_AddPolarOffset
	mov r2, r9
	mov r0, r9
	ldr r1, [r2]
	ldr r3, [r0, #8]
	ldr r2, [r2, #4]
	mov r0, r10
	bl Object_SetMoveTargetFar
	mov r0, r10
	bl BattleFx_SnapScaleToFull
	movs r3, #128
	mov r1, r10
	lsls r3, r3, #11
	str r3, [r1, #48]
	str r7, [r1, #52]
	movs r3, #4
	adds r1, #85
	str r1, [sp, #0]
	strb r3, [r1]
	ldr r3, .L_08097f58
	str r3, [r6, #108]
	ldr r3, .L_08097f5c
	str r3, [r6, #48]
	ldr r3, .L_08097f60
	add r2, sp, #4
	str r3, [r6, #52]
	ldrb r2, [r2]
	adds r3, r6, #0
	adds r3, #90
	strb r2, [r3]
	adds r2, r6, #0
	adds r2, #34
	movs r3, #2
	mov r7, r9
	mov r11, r5
	strb r3, [r2]
	b .L_08097ee4
.L_08097d00:
	ldr r3, .L_08097f64
	ldr r0, [r3]
	bl BattleFx_GetCycledTableWord
	lsls r0, r0, #16
	lsrs r0, r0, #16
	ldr r3, .L_08097f68
	mov r8, r0
	cmp r8, r3
	bne .L_08097d4a
	ldr r3, [r6, #8]
	str r3, [r7]
	ldr r3, [r6, #12]
	add r3, r11
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	str r3, [r7, #8]
	ldr r1, [sp, #8]
	mov r0, r11
	adds r2, r7, #0
	bl Vector_AddPolarOffset
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	mov r0, r10
	bl Object_SetMoveTargetFar
	mov r0, r10
	movs r1, #1
	bl Func_08009080
	mov r0, r10
	str r5, [r0, #36]
	str r5, [r0, #40]
	str r5, [r0, #44]
	b .L_08097ee4
.L_08097d4a:
	ldr r3, [r6, #8]
	str r3, [r7]
	ldr r3, [r6, #12]
	add r3, r11
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	str r3, [r7, #8]
	ldr r1, [sp, #8]
	mov r0, r11
	adds r2, r7, #0
	bl Vector_AddPolarOffset
	movs r0, #128
	lsls r0, r0, #10
	mov r1, r8
	adds r2, r7, #0
	bl Vector_AddPolarOffset
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	mov r0, r10
	bl Object_SetMoveTargetFar
	mov r0, r10
	bl Object_CommitPosition
	ldr r3, [r6, #8]
	str r3, [r7]
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	mov r0, r11
	str r3, [r7, #8]
	mov r1, r8
	adds r2, r7, #0
	bl Vector_AddPolarOffset
	ldr r3, [r6, #8]
	add r5, sp, #28
	str r3, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	movs r0, #128
	lsls r0, r0, #14
	mov r1, r8
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r6, #0
	adds r1, r7, #0
	bl Object_CheckMovementCollision
	cmp r0, #0
	bgt .L_08097e16
	adds r0, r6, #0
	mov r1, r9
	bl ScriptObject_FindOverlappingEntryFar
	cmp r0, #0
	beq .L_08097e36
	ldr r1, [sp, #20]
	cmp r0, r1
	bne .L_08097e16
	ldr r2, [sp, #20]
	ldr r3, [sp, #20]
	mov r1, r9
	ldr r0, [r2, #8]
	ldr r4, [r3, #16]
	ldr r2, .L_08097f6c
	ldr r3, [r1]
	ands r0, r2
	ands r3, r2
	ands r4, r2
	cmp r0, r3
	bne .L_08097dee
	ldr r3, [r1, #8]
	ands r3, r2
	cmp r4, r3
	beq .L_08097e16
.L_08097dee:
	ldr r1, [r5]
	ldr r2, .L_08097f6c
	adds r3, r1, #0
	ands r3, r2
	mov r12, r2
	cmp r0, r3
	bne .L_08097e36
	ldr r2, [r5, #8]
	mov r0, r12
	adds r3, r2, #0
	ands r3, r0
	cmp r4, r3
	bne .L_08097e36
	ldr r3, [sp, #20]
	adds r3, #34
	ldrb r0, [r3]
	bl Map_GetCellAttributeLowNibbleFar
	cmp r0, #0
	beq .L_08097e32
.L_08097e16:
	mov r0, r10
	movs r1, #4
	bl Func_08009080
	ldr r3, .L_08097f70
	ldr r3, [r3]
	movs r2, #15
	ands r3, r2
	cmp r3, #0
	bne .L_08097ee4
	movs r0, #114
	bl Func_080f9010
	b .L_08097ee4
.L_08097e32:
	movs r1, #1
	str r1, [sp, #4]
.L_08097e36:
	movs r0, #175
	bl Func_080f9010
	ldr r2, [r7]
	str r2, [sp, #16]
	ldr r0, [sp, #8]
	ldr r3, [r7, #8]
	mov r1, r8
	str r3, [sp, #12]
	subs r3, r0, r1
	ldr r2, .L_08097f74
	lsls r3, r3, #16
	lsrs r3, r3, #30
	ldrb r1, [r2, r3]
	mov r0, r10
	bl Func_08009080
	movs r0, #15
	bl WaitFrames
	adds r3, r6, #0
	adds r3, #91
	movs r2, #0
	strb r2, [r3]
	ldr r3, .L_08097f60
	str r3, [r6, #48]
	str r3, [r6, #52]
	mov r9, r3
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl Object_SetMoveTargetFar
	ldr r1, [sp, #0]
	mov r3, r10
	mov r2, r9
	movs r0, #0
	strb r0, [r1]
	str r2, [r3, #48]
	str r2, [r3, #52]
	mov r0, r11
	mov r1, r8
	adds r2, r7, #0
	bl Vector_AddPolarOffset
	ldr r2, [r7, #4]
	mov r0, r10
	ldr r1, [r7]
	add r2, r11
	ldr r3, [r7, #8]
	bl Object_SetMoveTargetFar
	ldr r0, [sp, #4]
	cmp r0, #1
	bne .L_08097ece
	ldr r2, [sp, #24]
	movs r1, #24
	ldrsh r0, [r2, r1]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	ldr r0, [sp, #20]
	mov r3, r9
	str r3, [r0, #48]
	str r3, [r0, #52]
	ldr r0, [sp, #20]
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	bl Object_SetMoveTargetFar
.L_08097ece:
	adds r0, r6, #0
	bl Object_CommitPosition
	ldr r1, [sp, #16]
	str r1, [r6, #8]
	ldr r2, [sp, #12]
	movs r3, #0
	str r2, [r6, #16]
	str r3, [r6, #36]
	str r3, [r6, #44]
	b .L_08097ef8
.L_08097ee4:
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_08097f78
	ldr r5, [r3]
	ldr r3, .L_08097f7c
	ands r5, r3
	cmp r5, #0
	bne .L_08097ef8
	b .L_08097d00
.L_08097ef8:
	ldr r3, [sp, #24]
	adds r3, #68
	ldrb r1, [r3]
	adds r0, r6, #0
	bl Animation_ApplyChildValuesFar
	ldr r0, [sp, #24]
	ldr r1, [r0, #60]
	adds r0, r6, #0
	bl Engine_ObjectSetScript
	ldr r1, [sp, #24]
	ldr r3, [r1, #56]
	str r3, [r6, #108]
	bl EffectRuntime_StopCurrentObject
	ldr r2, [sp, #4]
	cmp r2, #1
	bne .L_08097f32
	ldr r1, [sp, #24]
	movs r3, #24
	ldrsh r0, [r1, r3]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
.L_08097f32:
	bl BattleFx_PrepareBufferInterpolation
	mov r0, r10
	bl UpdateRisingParticleBurst
.L_08097f3c:
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_08097f50:
	.4byte Data_03001f30
.L_08097f54:
	.4byte Data_0809f0bc
.L_08097f58:
	.4byte ObjectGroup_ApplyRandomChildValues
.L_08097f5c:
	.4byte 0x00006666
.L_08097f60:
	.4byte 0x00003333
.L_08097f64:
	.4byte Data_03001ae8
.L_08097f68:
	.4byte 0x0000ffff
.L_08097f6c:
	.4byte 0xfff00000
.L_08097f70:
	.4byte gFrameCount
.L_08097f74:
	.4byte Data_0809f118
.L_08097f78:
	.4byte gKeyState
.L_08097f7c:
	.4byte 0x00000303
