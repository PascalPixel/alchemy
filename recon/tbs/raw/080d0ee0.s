.syntax unified
	.thumb
	.global Region_080d0ee0
	.thumb_func
Region_080d0ee0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .L_080d126c
	adds r3, r6, #0
	adds r5, r0, #0
	ldmia r3!, {r0}
	sub sp, #116
	str r0, [sp, #76]
	ldr r3, [r3]
	str r3, [sp, #72]
	ldr r1, [r6, #8]
	adds r3, r6, #0
	str r1, [sp, #52]
	subs r3, #108
	ldr r3, [r3]
	str r3, [sp, #48]
	ldr r0, [r5, #8]
	bl GetBattleObjectSlotFar
	ldr r3, .L_080d1270
	ldr r2, [sp, #76]
	ldr r0, [r0]
	adds r2, r2, r3
	str r5, [r2]
	mov r10, r0
	movs r0, #1
	mov r8, r2
	bl BattleFx_BeginCanvasLayer
	ldr r0, .L_080d1274
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080d1278
	movs r2, #128
	lsls r0, r0, #19
	bl _call_via_r3
	ldr r0, .L_080d127c
	bl Resource_GetTableEntry
	ldr r1, [sp, #52]
	bl Resource_DecodeType01
	mov r0, r10
	movs r1, #2
	bl Func_08009080
	mov r0, r10
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildrenFar
	movs r5, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r5, [sp, #0]
	bl Unnamed_080ed408
	movs r0, #239
	ldr r4, [sp, #76]
	lsls r0, r0, #7
	ldr r1, .L_080d1280
	adds r3, r4, r0
	adds r2, r4, r1
	str r5, [r3]
	ldr r6, [r6, #28]
	movs r3, #75
	movs r1, #144
	lsls r1, r1, #3
	str r3, [r2]
	ldr r0, .L_080d1284
	str r6, [sp, #60]
	bl Scheduler_AddOrUpdateCallback
	mov r2, r8
	ldr r3, [r2]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	mov r4, r8
	ldr r3, [r4]
	ldr r6, [r0]
	movs r1, #36
	ldrsh r0, [r3, r1]
	bl GetBattleObjectSlotFar
	movs r2, #0
	ldr r0, [r0]
	ldr r3, [sp, #76]
	movs r4, #225
	lsls r4, r4, #7
	str r2, [sp, #56]
	mov r11, r0
	adds r5, r3, r4
.L_080d0fac:
	ldr r2, [r6, #8]
	str r2, [r5]
	movs r0, #160
	ldr r3, [r6, #12]
	lsls r0, r0, #14
	adds r3, r3, r0
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	ldr r1, [sp, #56]
	cmp r1, #0
	bne .L_080d0fce
	mov r3, r11
	ldr r0, [r3, #8]
	movs r1, #12
	subs r0, r0, r2
	b .L_080d0fd8
.L_080d0fce:
	mov r4, r11
	ldr r0, [r4, #8]
	lsls r0, r0, #1
	subs r0, r0, r2
	movs r1, #12
.L_080d0fd8:
	bl FixedPoint_Ratio
	str r0, [r5, #12]
	mov r1, r11
	ldr r0, [r1, #12]
	ldr r3, [r5, #4]
	movs r2, #160
	subs r0, r0, r3
	lsls r2, r2, #14
	adds r0, r0, r2
	movs r1, #12
	bl FixedPoint_Ratio
	str r0, [r5, #16]
	mov r3, r11
	ldr r0, [r3, #16]
	ldr r3, [r5, #8]
	movs r1, #12
	subs r0, r0, r3
	bl FixedPoint_Ratio
	movs r3, #0
	str r0, [r5, #20]
	str r3, [r5, #24]
	ldr r4, [sp, #56]
	adds r4, #1
	adds r5, #28
	str r4, [sp, #56]
	cmp r4, #3
	bne .L_080d0fac
	str r3, [sp, #68]
.L_080d1016:
	ldr r0, [sp, #68]
	cmp r0, #47
	bgt .L_080d104a
	ldr r3, .L_080d1288
	movs r2, #128
	ldr r1, [r3]
	cmp r0, #39
	ble .L_080d1030
	ldr r3, [sp, #68]
	lsls r2, r3, #4
	movs r3, #192
	lsls r3, r3, #2
	subs r2, r3, r2
.L_080d1030:
	ldr r4, [sp, #76]
	ldr r0, .L_080d1270
	adds r3, r4, r0
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080d1044
	ldrh r3, [r1, #54]
	subs r3, r3, r2
	b .L_080d1048
.L_080d1044:
	ldrh r3, [r1, #54]
	adds r3, r3, r2
.L_080d1048:
	strh r3, [r1, #54]
.L_080d104a:
	movs r3, #225
	ldr r2, [sp, #76]
	lsls r3, r3, #7
	movs r1, #0
	adds r3, r2, r3
	str r1, [sp, #56]
	str r1, [sp, #24]
	str r3, [sp, #20]
	str r1, [sp, #16]
.L_080d105c:
	ldr r4, [sp, #68]
	ldr r0, [sp, #16]
	cmp r4, r0
	bge .L_080d1066
	b .L_080d12d2
.L_080d1066:
	ldr r1, [sp, #20]
	subs r5, r4, r0
	mov r10, r1
	adds r3, r5, #0
	cmp r5, #0
	bge .L_080d1074
	adds r3, r5, #3
.L_080d1074:
	asrs r3, r3, #2
	adds r3, #2
	mov r8, r3
	cmp r3, #10
	ble .L_080d1082
	movs r2, #10
	mov r8, r2
.L_080d1082:
	bl Render_ResetTransformState
	ldr r0, [sp, #48]
	adds r1, r0, #0
	adds r1, #12
	bl Graphics_PrepareTransferInIwramWork
	ldr r0, [sp, #20]
	bl SceneTransform_ApplyPosition
	movs r3, #0
	ldr r2, [sp, #24]
	movs r0, #128
	str r3, [sp, #44]
	lsls r0, r0, #5
	lsls r3, r5, #12
	adds r0, r3, r0
	lsls r3, r2, #3
	str r0, [sp, #36]
	movs r1, #92
	subs r3, r3, r2
	ldr r0, .L_080d128c
	add r1, sp
	lsls r3, r3, #2
	str r5, [sp, #40]
	movs r4, #0
	add r7, sp, #80
	add r6, sp, #104
	mov r9, r1
	adds r5, r3, r0
.L_080d10be:
	str r4, [sp, #12]
	bl Graphics_SaveTransferWorkOnce
	ldr r1, [sp, #40]
	lsls r0, r1, #10
	bl SceneTransform_ApplyRoll
	movs r0, #128
	lsls r0, r0, #7
	bl SceneTransform_ApplyYaw
	movs r3, #128
	ldr r2, [sp, #36]
	lsls r3, r3, #9
	str r2, [r7]
	ldr r4, [sp, #12]
	cmp r2, r3
	ble .L_080d10e4
	str r3, [r7]
.L_080d10e4:
	ldr r3, [r7]
	adds r0, r7, #0
	str r3, [r7, #4]
	str r3, [r7, #8]
	str r4, [sp, #12]
	bl SceneTransform_ApplyScale
	ldr r3, .L_080d1290
	ldr r4, [sp, #12]
	adds r0, r4, #0
	muls r0, r3
	bl SceneTransform_ApplyRoll
	ldr r4, [sp, #12]
	movs r3, #1
	ands r3, r4
	lsls r0, r3, #1
	adds r0, r0, r3
	ldr r3, .L_080d1294
	lsls r0, r0, #2
	adds r0, r0, r3
	adds r1, r6, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [sp, #44]
	ldr r4, [sp, #12]
	cmp r3, r0
	bge .L_080d111e
	str r0, [sp, #44]
.L_080d111e:
	ldr r2, [r6]
	asrs r2, r2, #1
	str r2, [r6]
	mov r0, r9
	ldr r3, [r0]
	adds r2, r2, r3
	str r2, [r5, #12]
	ldr r3, [r0, #4]
	ldr r2, [r6, #4]
	adds r2, r2, r3
	str r2, [r5, #16]
	ldr r3, [r6]
	str r3, [r5, #12]
	ldr r3, [r6, #4]
	str r3, [r5, #16]
	str r4, [sp, #12]
	bl Graphics_RestoreTransferWork
	ldr r4, [sp, #12]
	adds r4, #1
	adds r5, #28
	cmp r4, #10
	bne .L_080d10be
	ldr r1, [sp, #44]
	ldr r2, .L_080d1298
	cmp r1, r2
	bgt .L_080d11ea
	ldr r3, [sp, #24]
	mov r1, r8
	str r3, [sp, #32]
	lsrs r3, r1, #31
	add r3, r8
	mov r0, r8
	asrs r3, r3, #1
	lsls r0, r0, #1
	str r3, [sp, #28]
	movs r4, #0
	mov r9, r0
.L_080d116a:
	ldr r3, [sp, #32]
	adds r2, r4, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r0, .L_080d128c
	lsls r3, r3, #2
	adds r4, #1
	adds r7, r3, r0
	movs r1, #10
	adds r0, r4, #0
	str r4, [sp, #8]
	bl Func_080022fc
	ldr r1, [sp, #32]
	adds r0, r0, r1
	lsls r3, r0, #3
	subs r3, r3, r0
	ldr r2, .L_080d128c
	lsls r3, r3, #2
	adds r6, r3, r2
	movs r5, #0
.L_080d1194:
	ldr r3, [r6, #12]
	ldr r2, [r7, #12]
	subs r3, r3, r2
	muls r3, r5
	cmp r3, #0
	bge .L_080d11a2
	adds r3, #15
.L_080d11a2:
	asrs r3, r3, #4
	adds r0, r2, r3
	ldr r3, [r6, #16]
	ldr r2, [r7, #16]
	subs r3, r3, r2
	muls r3, r5
	cmp r3, #0
	bge .L_080d11b4
	adds r3, #15
.L_080d11b4:
	asrs r3, r3, #4
	adds r2, r2, r3
	mov r12, r2
	ldr r3, .L_080d129c
	mov r2, r9
	subs r2, #2
	ldrh r1, [r3, r2]
	ldr r4, [sp, #52]
	ldr r3, [sp, #28]
	adds r1, r4, r1
	subs r2, r0, r3
	mov r4, r12
	mov r0, r8
	subs r3, r4, r0
	mov r4, r9
	str r0, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #72]
	ldr r4, [sp, #60]
	adds r5, #1
	bl _call_via_r4
	cmp r5, #16
	bne .L_080d1194
	ldr r4, [sp, #8]
	cmp r4, #10
	bne .L_080d116a
.L_080d11ea:
	mov r0, r10
	ldr r3, [r0]
	ldr r2, [r0, #12]
	adds r3, r3, r2
	str r3, [r0]
	ldr r2, [r0, #16]
	ldr r3, [r0, #4]
	adds r3, r3, r2
	str r3, [r0, #4]
	ldr r2, [r0, #20]
	ldr r3, [r0, #8]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r1, [sp, #16]
	ldr r2, [sp, #56]
	ldr r4, [sp, #68]
	adds r3, r1, r2
	adds r3, #10
	cmp r4, r3
	bne .L_080d12d2
	movs r3, #128
	mov r0, r11
	lsls r3, r3, #10
	str r3, [r0, #52]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r0, #48]
	movs r3, #160
	lsls r3, r3, #11
	str r3, [r0, #40]
	ldr r3, .L_080d12a0
	mov r2, r11
	str r3, [r0, #72]
	adds r2, #90
	movs r3, #0
	strb r3, [r2]
	bl Object_ResetMotion
	mov r2, r11
	ldr r1, [r2, #8]
	cmp r1, #0
	bge .L_080d124e
	ldr r3, .L_080d12a4
	mov r0, r11
	adds r1, r1, r3
	ldr r3, [r2, #16]
	movs r2, #0
	bl Object_SetMoveTargetFar
	b .L_080d125e
.L_080d124e:
	movs r4, #160
	lsls r4, r4, #14
	mov r0, r11
	adds r1, r1, r4
	ldr r3, [r0, #16]
	movs r2, #0
	bl Object_SetMoveTargetFar
.L_080d125e:
	ldr r1, [sp, #56]
	cmp r1, #2
	bne .L_080d12a8
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
	b .L_080d12c8
.L_080d126c:
	.4byte gBattleFxWork
.L_080d1270:
	.4byte 0x00007828
.L_080d1274:
	.4byte 0x00000079
.L_080d1278:
	.4byte IwramCopyWords
.L_080d127c:
	.4byte 0x00000073
.L_080d1280:
	.4byte 0x00007784
.L_080d1284:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d1288:
	.4byte gCameraWork
.L_080d128c:
	.4byte gMapCellBuffer
.L_080d1290:
	.4byte 0x0000199a
.L_080d1294:
	.4byte Data_080ee140
.L_080d1298:
	.4byte 0x00061a7f
.L_080d129c:
	.4byte ParticleStreams_CellOffsets
.L_080d12a0:
	.4byte 0x0000ab85
.L_080d12a4:
	.4byte 0xffd80000
.L_080d12a8:
	movs r0, #134
	bl Func_080f9010
	ldr r4, .L_080d1340
	ldr r2, [sp, #76]
	adds r3, r2, r4
	ldr r3, [r3]
	movs r1, #36
	ldrsh r0, [r3, r1]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl ObjectGroup_UpdateMembers
.L_080d12c8:
	ldr r3, [sp, #76]
	ldr r4, .L_080d1344
	adds r2, r3, r4
	movs r3, #4
	str r3, [r2]
.L_080d12d2:
	ldr r0, [sp, #24]
	ldr r1, [sp, #20]
	ldr r2, [sp, #16]
	ldr r3, [sp, #56]
	adds r0, #10
	adds r1, #28
	adds r2, #12
	adds r3, #1
	str r0, [sp, #24]
	str r1, [sp, #20]
	str r2, [sp, #16]
	str r3, [sp, #56]
	cmp r3, #3
	beq .L_080d12f0
	b .L_080d105c
.L_080d12f0:
	movs r1, #8
	movs r0, #8
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r0, .L_080d1348
	ldr r4, [sp, #76]
	movs r3, #1
	adds r2, r4, r0
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #68]
	adds r1, #1
	str r1, [sp, #68]
	cmp r1, #60
	beq .L_080d1318
	b .L_080d1016
.L_080d1318:
	ldr r0, .L_080d134c
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #116
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080d1340:
	.4byte 0x00007828
.L_080d1344:
	.4byte 0x000077a8
.L_080d1348:
	.4byte 0x00007824
.L_080d134c:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
