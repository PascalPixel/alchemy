.syntax unified
	.thumb
	.global Func_080a414c
	.thumb_func
Func_080a414c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #0
	sub sp, #16
	mov r10, r1
	movs r1, #8
	ldr r3, .L_080a4468
	add r1, sp
	mov r11, r1
	movs r2, #0
	ldr r6, [r3]
	mov r0, r11
	movs r3, #1
	mov r8, r2
	mov r9, r3
	bl ItemMenu_BuildCmd
	movs r2, #136
	lsls r2, r2, #2
	adds r2, r6, r2
	str r2, [sp, #4]
	ldrh r3, [r2]
	movs r7, #0
	cmp r3, #1
	beq .L_080a41e0
	bl ItemMenu_HideAllIcons
	ldr r0, [r6, #52]
	bl RenderOutput_RedrawSavedRectFar
	movs r1, #134
	lsls r1, r1, #1
	adds r3, r6, r1
	ldr r5, [r3]
	bl ItemMenu_SetMsgWin7
	adds r0, r5, #0
	bl RenderOutput_RedrawSavedRectFar
	movs r3, #3
	str r3, [sp, #0]
	movs r2, #3
	movs r3, #16
	adds r0, r5, #0
	movs r1, #0
	bl UiWindow_DrawDividerLineFar
	bl ItemMenu_DrawItemHead
	adds r1, r5, #0
	mov r0, r11
	bl ItemMenu_DrawCmd
	ldr r0, [r6, #44]
	bl RenderOutput_RedrawSavedRectFar
	movs r2, #188
	lsls r2, r2, #1
	adds r3, r6, r2
	ldrh r3, [r3]
	ldr r0, .L_080a446c
	ands r0, r3
	ldr r3, .L_080a4470
	ldr r1, [r6, #44]
	adds r0, r0, r3
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_080a41e0:
	ldr r1, [sp, #4]
	mov r3, r10
	ldr r2, .L_080a4474
	strh r3, [r1]
	adds r3, r6, r2
	movs r5, #0
	ldrsb r5, [r3, r5]
	movs r3, #1
	negs r3, r3
	cmp r5, r3
	bne .L_080a4258
	mov r1, r11
	movs r3, #2
	ldrsb r3, [r1, r3]
	cmp r3, #1
	bne .L_080a4206
	movs r2, #0
	movs r7, #2
	mov r8, r2
.L_080a4206:
	mov r1, r11
	movs r3, #3
	ldrsb r3, [r1, r3]
	cmp r3, #1
	bne .L_080a4216
	movs r2, #1
	movs r7, #0
	mov r8, r2
.L_080a4216:
	mov r1, r11
	movs r3, #1
	ldrsb r3, [r1, r3]
	cmp r3, #1
	bne .L_080a4226
	movs r2, #0
	movs r7, #1
	mov r8, r2
.L_080a4226:
	mov r1, r11
	movs r3, #4
	ldrsb r3, [r1, r3]
	cmp r3, #1
	bne .L_080a4236
	movs r2, #1
	movs r7, #1
	mov r8, r2
.L_080a4236:
	mov r1, r11
	movs r3, #0
	ldrsb r3, [r1, r3]
	cmp r3, #1
	bne .L_080a427a
	movs r2, #0
	movs r7, #0
	mov r8, r2
	b .L_080a427a
.L_080a4248:
	movs r0, #113
	bl AudioCommand_PlayFar
	movs r3, #1
	negs r3, r3
	ldr r1, .L_080a4474
	mov r10, r3
	b .L_080a43c0
.L_080a4258:
	movs r1, #3
	adds r0, r5, #0
	bl __modsi3
	lsls r0, r0, #24
	asrs r7, r0, #24
	movs r1, #3
	adds r0, r5, #0
	bl __divsi3
	lsls r0, r0, #24
	asrs r0, r0, #24
	mov r8, r0
	lsls r3, r0, #1
	add r3, r8
	adds r3, r3, r7
	mov r10, r3
.L_080a427a:
	mov r1, r8
	adds r0, r7, #0
	bl ItemMenu_CmdCursorX
	mov r1, r8
	adds r5, r0, #0
	adds r0, r7, #0
	bl ItemMenu_CmdCursorY
	adds r1, r0, #0
	adds r0, r5, #0
	bl UiMenu_SlideCursor
	b .L_080a4436
.L_080a4296:
	mov r3, r9
	cmp r3, #0
	beq .L_080a4330
	movs r1, #0
	adds r0, r7, #3
	mov r9, r1
	movs r1, #3
	bl __modsi3
	mov r2, r8
	adds r2, #2
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	lsls r3, r3, #1
	subs r2, r2, r3
	mov r8, r2
	lsls r3, r2, #1
	adds r7, r0, #0
	add r3, r8
	adds r3, r3, r7
	mov r10, r3
	bl EquipmentMenu_StartCompatibilityIndicators
	mov r2, r10
	cmp r2, #2
	ble .L_080a42fc
	movs r3, #151
	lsls r3, r3, #2
	adds r2, r6, r3
	ldr r1, .L_080a4478
	movs r3, #1
	strb r3, [r2]
	adds r3, r6, r1
	ldrb r3, [r3]
	subs r1, #166
	adds r2, r6, r1
	ldrh r1, [r2]
	adds r0, r3, #0
	movs r2, #0
	bl ItemMenu_DrawEquipPreview
	mov r2, r10
	cmp r2, #3
	bne .L_080a4330
	movs r1, #200
	ldr r0, .L_080a447c
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	b .L_080a4330
.L_080a42fc:
	mov r3, r10
	cmp r3, #0
	beq .L_080a4320
	movs r1, #151
	lsls r1, r1, #2
	adds r3, r6, r1
	ldr r2, .L_080a4478
	strb r5, [r3]
	adds r3, r6, r2
	subs r1, #232
	ldrb r3, [r3]
	adds r2, r6, r1
	ldrh r1, [r2]
	adds r0, r3, #0
	movs r2, #0
	bl ItemMenu_DrawEquipPreview
	b .L_080a4330
.L_080a4320:
	ldr r2, .L_080a4478
	adds r3, r6, r2
	ldrb r1, [r3]
	ldr r0, [r6, #36]
	movs r2, #0
	movs r3, #0
	bl Menu_DrawOwnerStatusPanel
.L_080a4330:
	mov r1, r8
	adds r0, r7, #0
	bl ItemMenu_CmdCursorX
	mov r1, r8
	adds r5, r0, #0
	adds r0, r7, #0
	bl ItemMenu_CmdCursorY
	adds r1, r0, #0
	adds r0, r5, #0
	bl UiMenu_PositionCursor
	movs r0, #1
	bl WaitFrames
	ldr r5, .L_080a4480
	ldr r2, [r5]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_080a43c8
	mov r1, r11
	mov r2, r10
	ldrsb r3, [r1, r2]
	movs r1, #1
	negs r1, r1
	cmp r3, r1
	bne .L_080a4372
	movs r0, #114
	bl AudioCommand_PlayFar
	b .L_080a43c8
.L_080a4372:
	mov r2, r10
	cmp r2, #5
	bhi .L_080a43b8
	lsls r3, r2, #2
	ldr r2, .L_080a4484
	ldr r3, [r3, r2]
	mov pc, r3
.L_080a4380:
	.4byte .L_080a4398
	.4byte .L_080a43a0
	.4byte .L_080a43a8
	.4byte .L_080a43a8
	.4byte .L_080a43b0
	.4byte .L_080a43a8
.L_080a4398:
	movs r0, #174
	bl AudioCommand_PlayFar
	b .L_080a43be
.L_080a43a0:
	movs r0, #175
	bl AudioCommand_PlayFar
	b .L_080a43be
.L_080a43a8:
	movs r0, #112
	bl AudioCommand_PlayFar
	b .L_080a43be
.L_080a43b0:
	movs r0, #117
	bl AudioCommand_PlayFar
	b .L_080a43be
.L_080a43b8:
	movs r0, #112
	bl AudioCommand_PlayFar
.L_080a43be:
	ldr r1, .L_080a4474
.L_080a43c0:
	mov r2, r10
	adds r3, r6, r1
	strb r2, [r3]
	b .L_080a4446
.L_080a43c8:
	ldr r2, [r5]
	movs r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_080a43d4
	b .L_080a4248
.L_080a43d4:
	ldr r1, .L_080a4488
	ldr r2, [r1]
	movs r3, #64
	ands r2, r3
	cmp r2, #0
	beq .L_080a43f0
	subs r3, #65
	movs r1, #1
	movs r0, #111
	add r8, r3
	mov r9, r1
	bl AudioCommand_PlayFar
	b .L_080a4436
.L_080a43f0:
	ldr r2, [r1]
	movs r3, #128
	ands r2, r3
	cmp r2, #0
	beq .L_080a4408
	movs r2, #1
	movs r0, #111
	add r8, r2
	mov r9, r2
	bl AudioCommand_PlayFar
	b .L_080a4436
.L_080a4408:
	ldr r2, [r1]
	movs r3, #16
	ands r2, r3
	cmp r2, #0
	beq .L_080a4420
	movs r3, #1
	movs r0, #111
	adds r7, #1
	mov r9, r3
	bl AudioCommand_PlayFar
	b .L_080a4436
.L_080a4420:
	ldr r3, [r1]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_080a4436
	movs r1, #1
	movs r0, #111
	subs r7, #1
	mov r9, r1
	bl AudioCommand_PlayFar
.L_080a4436:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_IsSet
	adds r5, r0, #0
	cmp r5, #0
	bne .L_080a4446
	b .L_080a4296
.L_080a4446:
	movs r3, #151
	lsls r3, r3, #2
	adds r2, r6, r3
	movs r3, #0
	strb r3, [r2]
	bl EquipmentMenu_StartCompatibilityIndicators
	mov r0, r10
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_080a4468:
	.4byte gMenuWork
.L_080a446c:
	.4byte 0x000001ff
.L_080a4470:
	.4byte 0x00000075
.L_080a4474:
	.4byte 0x0000025d
.L_080a4478:
	.4byte 0x0000021a
.L_080a447c:
	.4byte EquipmentMenu_UpdateCompatibilityIndicators
.L_080a4480:
	.4byte gKeyState
.L_080a4484:
	.4byte .L_080a4380
.L_080a4488:
	.4byte gKeysRepeat
