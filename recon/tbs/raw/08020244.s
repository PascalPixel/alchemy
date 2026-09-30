.syntax unified
	.thumb
	.global SaveMenu_SelectSlot
	.thumb_func
SaveMenu_SelectSlot:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r9, r1
	movs r1, #167
	mov r8, r0
	lsls r1, r1, #4
	movs r0, #55
	sub sp, #40
	bl Runtime_AllocateHeapBlock
	ldr r3, .L_0802057c
	movs r1, #0
	movs r2, #1
	ldr r7, [r3]
	str r1, [sp, #32]
	str r1, [sp, #28]
	str r1, [sp, #24]
	str r2, [sp, #12]
	subs r3, #144
	ldr r3, [r3]
	adds r5, r0, #0
	str r3, [sp, #8]
	bl Runtime_GetBuildStampTimeFar
	mov r3, r8
	str r0, [sp, #4]
	cmp r3, #0
	bge .L_0802028a
	movs r1, #0
	mov r8, r1
.L_0802028a:
	mov r2, r9
	cmp r2, #1
	bne .L_080202d2
	mov r3, r8
	ldr r1, .L_08020580
	lsls r2, r3, #6
	adds r3, r2, r1
	ldrb r3, [r7, r3]
	movs r6, #0
	b .L_080202bc
.L_0802029e:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #3
	bne .L_080202ac
	movs r1, #0
	mov r8, r1
.L_080202ac:
	adds r6, #1
	cmp r6, #2
	bgt .L_0802039a
	mov r3, r8
	ldr r1, .L_08020580
	lsls r2, r3, #6
	adds r3, r2, r1
	ldrb r3, [r7, r3]
.L_080202bc:
	cmp r3, #0
	beq .L_0802029e
	adds r1, #20
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0802029e
	b .L_0802039a
.L_080202d2:
	mov r2, r9
	cmp r2, #4
	bne .L_0802031a
	mov r3, r8
	ldr r1, .L_08020580
	lsls r2, r3, #6
	adds r3, r2, r1
	ldrb r3, [r7, r3]
	movs r6, #0
	b .L_08020304
.L_080202e6:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #3
	bne .L_080202f4
	movs r1, #0
	mov r8, r1
.L_080202f4:
	adds r6, #1
	cmp r6, #2
	bgt .L_0802039a
	mov r3, r8
	ldr r1, .L_08020580
	lsls r2, r3, #6
	adds r3, r2, r1
	ldrb r3, [r7, r3]
.L_08020304:
	cmp r3, #0
	beq .L_080202e6
	adds r1, #20
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080202e6
	b .L_0802039a
.L_0802031a:
	mov r2, r9
	cmp r2, #5
	bne .L_08020362
	mov r3, r8
	ldr r1, .L_08020580
	lsls r2, r3, #6
	adds r3, r2, r1
	ldrb r3, [r7, r3]
	movs r6, #0
	b .L_0802034c
.L_0802032e:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #3
	bne .L_0802033c
	movs r1, #0
	mov r8, r1
.L_0802033c:
	adds r6, #1
	cmp r6, #2
	bgt .L_0802039a
	mov r3, r8
	ldr r1, .L_08020580
	lsls r2, r3, #6
	adds r3, r2, r1
	ldrb r3, [r7, r3]
.L_0802034c:
	cmp r3, #0
	beq .L_0802032e
	adds r1, #20
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_0802032e
	b .L_0802039a
.L_08020362:
	mov r2, r9
	cmp r2, #0
	beq .L_080203b0
	mov r3, r8
	ldr r1, .L_08020580
	lsls r2, r3, #6
	adds r3, r2, r1
	ldrb r3, [r7, r3]
	movs r6, #0
	cmp r3, #0
	bne .L_0802039a
	adds r3, r2, r7
	adds r2, r3, r1
.L_0802037c:
	movs r3, #1
	add r8, r3
	mov r3, r8
	adds r2, #64
	cmp r3, #3
	bne .L_0802038e
	movs r3, #0
	adds r2, r7, r1
	mov r8, r3
.L_0802038e:
	adds r6, #1
	cmp r6, #2
	bgt .L_0802039a
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_0802037c
.L_0802039a:
	cmp r6, #3
	bne .L_080203b0
	movs r5, #2
	negs r5, r5
	b .L_08020794
.L_080203a4:
	movs r0, #113
	movs r5, #1
	bl Func_080f9010
	negs r5, r5
	b .L_0802074a
.L_080203b0:
	add r0, sp, #36
	movs r3, #0
	str r3, [r0]
	adds r1, r5, #0
	ldr r3, .L_08020584
	ldr r2, .L_08020588
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bl Scheduler_ScheduleCallbackAAfterFrames
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #2
	movs r2, #28
	movs r3, #7
	movs r0, #1
	bl UiWindow_Create
	movs r1, #130
	lsls r1, r1, #5
	adds r1, r7, r1
	ldr r3, .L_08020580
	movs r2, #0
	str r1, [sp, #20]
	mov r10, r0
	mov r11, r2
	movs r6, #2
	adds r5, r7, r3
.L_080203e8:
	ldrb r3, [r5]
	cmp r3, #0
	bne .L_080203f2
	ldr r0, .L_0802058c
	b .L_0802041a
.L_080203f2:
	ldrh r3, [r5, #26]
	ldr r1, [sp, #4]
	cmp r3, r1
	bcs .L_080203fe
	ldr r0, .L_08020590
	b .L_0802041a
.L_080203fe:
	ldr r2, [r5, #4]
	ldr r3, [r5, #28]
	cmp r2, r3
	beq .L_0802040a
	ldr r0, .L_08020594
	b .L_0802041a
.L_0802040a:
	mov r2, r9
	cmp r2, #5
	bne .L_08020426
	movs r3, #21
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_08020426
	ldr r0, .L_08020598
.L_0802041a:
	mov r1, r10
	movs r2, #10
	mov r3, r11
	bl UiText_DrawResource
	b .L_0802044e
.L_08020426:
	ldr r0, [sp, #20]
	mov r1, r10
	adds r0, #16
	movs r2, #12
	mov r3, r11
	bl UiText_DrawString
	ldr r3, .L_0802059c
	ldrh r0, [r5, #2]
	mov r1, r10
	adds r0, r0, r3
	movs r2, #62
	mov r3, r11
	bl UiText_DrawResource
	ldr r1, .L_080205a0
	ldr r3, [sp, #8]
	adds r2, r3, r1
	movs r3, #1
	strb r3, [r2]
.L_0802044e:
	ldr r3, [sp, #20]
	movs r2, #16
	adds r3, #64
	subs r6, #1
	add r11, r2
	adds r5, #64
	str r3, [sp, #20]
	cmp r6, #0
	bge .L_080203e8
	movs r3, #2
	str r3, [sp, #0]
	mov r0, r10
	movs r1, #0
	movs r2, #2
	movs r3, #27
	bl UiWindow_DrawDividerLine
	movs r3, #4
	str r3, [sp, #0]
	mov r0, r10
	movs r1, #0
	movs r2, #4
	movs r3, #27
	bl UiWindow_DrawDividerLine
	movs r3, #24
	mov r1, r10
	negs r3, r3
	mov r0, r9
	movs r2, #72
	bl RenderResource_CreatePair
	movs r1, #2
	str r0, [sp, #16]
	mov r11, r1
.L_08020494:
	ldr r2, [sp, #12]
	cmp r2, #0
	bne .L_0802049c
	b .L_0802061e
.L_0802049c:
	movs r3, #0
	mov r1, r8
	ldr r2, .L_08020580
	str r3, [sp, #12]
	lsls r5, r1, #6
	adds r3, r5, r2
	ldrb r3, [r7, r3]
	cmp r3, #0
	bne .L_080204b0
	b .L_080205ac
.L_080204b0:
	ldr r1, .L_080205a4
	adds r3, r5, r1
	ldrb r0, [r7, r3]
	adds r3, r7, r3
	ldrb r1, [r3, #1]
	bl PaletteGlow_Update
	ldr r2, [sp, #32]
	cmp r2, #0
	bne .L_080204d6
	mov r3, r11
	str r3, [sp, #0]
	movs r0, #1
	movs r1, #10
	movs r2, #14
	movs r3, #9
	bl UiWindow_Create
	str r0, [sp, #32]
.L_080204d6:
	movs r1, #130
	adds r3, r7, r5
	lsls r1, r1, #5
	adds r6, r3, r1
	ldr r0, [sp, #32]
	adds r1, r6, #0
	bl StatusMenu_DrawCharacterSummary
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #28]
	cmp r2, #0
	bne .L_08020504
	mov r3, r11
	str r3, [sp, #0]
	movs r0, #16
	movs r1, #10
	movs r2, #13
	movs r3, #3
	bl UiWindow_Create
	str r0, [sp, #28]
.L_08020504:
	bl Menu_ClearFirstObjectRowAndScheduleUpdate
	movs r1, #0
	ldr r0, [sp, #28]
	movs r2, #0
	adds r3, r6, #0
	bl ObjectPlacement_CreateGroup
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_080205a8
	adds r3, r5, r1
	adds r1, #1
	ldrsb r2, [r7, r3]
	adds r3, r5, r1
	ldrsb r3, [r7, r3]
	adds r1, #1
	adds r2, r2, r3
	adds r3, r5, r1
	ldrsb r3, [r7, r3]
	adds r1, #1
	adds r2, r2, r3
	adds r3, r5, r1
	ldrsb r3, [r7, r3]
	cmn r2, r3
	beq .L_0802056a
	ldr r2, [sp, #24]
	cmp r2, #0
	bne .L_08020552
	mov r3, r11
	str r3, [sp, #0]
	movs r0, #16
	movs r1, #14
	movs r2, #13
	movs r3, #5
	bl UiWindow_Create
	str r0, [sp, #24]
.L_08020552:
	ldr r0, [sp, #24]
	adds r1, r6, #0
	bl UiText_DrawFourNumbersInRow
	bl Menu_ClearSecondObjectRowAndScheduleUpdate
	movs r1, #0
	ldr r0, [sp, #24]
	movs r2, #0
	bl Menu_SpawnFourObjectsAtOrigin
	b .L_080205e6
.L_0802056a:
	bl Menu_ClearSecondObjectRowAndScheduleUpdate
	movs r1, #2
	ldr r0, [sp, #24]
	bl UiWork_Finalize
	movs r1, #0
	str r1, [sp, #24]
	b .L_080205e6
.L_0802057c:
	.4byte gSaveWorkspace
.L_08020580:
	.4byte 0x0000105c
.L_08020584:
	.4byte 0x040000d4
.L_08020588:
	.4byte 0x8500029c
.L_0802058c:
	.4byte 0x00000000
.L_08020590:
	.4byte 0x00000001
.L_08020594:
	.4byte 0x00000003
.L_08020598:
	.4byte 0x00000002
.L_0802059c:
	.4byte 0x0000099b
.L_080205a0:
	.4byte 0x00000ea3
.L_080205a4:
	.4byte 0x00001074
.L_080205a8:
	.4byte 0x00001068
.L_080205ac:
	ldr r2, .L_080207a8
	ldr r1, .L_080207ac
	adds r3, r2, r1
	ldrb r0, [r3]
	ldr r3, .L_080207b0
	adds r2, r2, r3
	ldrb r1, [r2]
	bl PaletteGlow_Update
	bl Menu_ClearSecondObjectRowAndScheduleUpdate
	bl Menu_ClearFirstObjectRowAndScheduleUpdate
	movs r1, #2
	ldr r0, [sp, #24]
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [sp, #28]
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [sp, #32]
	bl UiWork_Finalize
	movs r1, #0
	str r1, [sp, #24]
	str r1, [sp, #28]
	str r1, [sp, #32]
.L_080205e6:
	mov r0, r10
	bl RenderOutput_RedrawSavedRect
	mov r2, r11
	str r2, [sp, #0]
	mov r0, r10
	movs r1, #0
	movs r2, #2
	movs r3, #27
	bl UiWindow_DrawDividerLine
	movs r3, #4
	str r3, [sp, #0]
	mov r0, r10
	movs r1, #0
	movs r2, #4
	movs r3, #27
	bl UiWindow_DrawDividerLine
	mov r3, r8
	lsls r2, r3, #1
	movs r3, #1
	str r3, [sp, #0]
	mov r0, r10
	movs r1, #0
	movs r3, #26
	bl UiWindow_FillTilemapRect
.L_0802061e:
	ldr r0, [sp, #16]
	bl Ui_ApplyTableOffsetToPair
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_080207b4
	ldr r2, [r1]
	movs r3, #64
	ands r2, r3
	cmp r2, #0
	beq .L_080206aa
	movs r0, #111
	bl Func_080f9010
	movs r1, #1
	str r1, [sp, #12]
	b .L_08020696
.L_08020642:
	mov r3, r8
	ldr r1, .L_080207b8
	lsls r2, r3, #6
	adds r3, r2, r1
	ldrb r3, [r7, r3]
	cmp r3, #0
	beq .L_08020696
	mov r3, r9
	cmp r3, #1
	bne .L_08020666
	adds r1, #20
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_08020696
.L_08020666:
	mov r3, r9
	cmp r3, #4
	bne .L_0802067c
	ldr r1, .L_080207bc
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_08020696
.L_0802067c:
	mov r3, r9
	cmp r3, #5
	beq .L_08020684
	b .L_08020494
.L_08020684:
	ldr r1, .L_080207bc
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_08020696
	b .L_08020494
.L_08020696:
	mov r0, r8
	adds r0, #2
	movs r1, #3
	bl Func_080022fc
	mov r2, r9
	mov r8, r0
	cmp r2, #0
	bne .L_08020642
	b .L_08020494
.L_080206aa:
	ldr r2, [r1]
	movs r3, #128
	ands r2, r3
	cmp r2, #0
	beq .L_08020728
	movs r0, #111
	bl Func_080f9010
	movs r3, #1
	str r3, [sp, #12]
	b .L_08020714
.L_080206c0:
	mov r1, r8
	lsls r2, r1, #6
	ldr r1, .L_080207b8
	adds r3, r2, r1
	ldrb r3, [r7, r3]
	cmp r3, #0
	beq .L_08020714
	mov r3, r9
	cmp r3, #1
	bne .L_080206e4
	adds r1, #20
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_08020714
.L_080206e4:
	mov r3, r9
	cmp r3, #4
	bne .L_080206fa
	ldr r1, .L_080207bc
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_08020714
.L_080206fa:
	mov r3, r9
	cmp r3, #5
	beq .L_08020702
	b .L_08020494
.L_08020702:
	ldr r1, .L_080207bc
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_08020714
	b .L_08020494
.L_08020714:
	mov r0, r8
	adds r0, #4
	movs r1, #3
	bl Func_080022fc
	mov r2, r9
	mov r8, r0
	cmp r2, #0
	bne .L_080206c0
	b .L_08020494
.L_08020728:
	ldr r2, .L_080207c0
	ldr r3, [r2]
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L_08020736
	b .L_080203a4
.L_08020736:
	ldr r3, [r2]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_08020742
	b .L_08020494
.L_08020742:
	movs r0, #112
	bl Func_080f9010
	mov r5, r8
.L_0802074a:
	bl Menu_ClearSecondObjectRowAndScheduleUpdate
	bl Menu_ClearFirstObjectRowAndScheduleUpdate
	movs r1, #2
	ldr r0, [sp, #24]
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [sp, #28]
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [sp, #32]
	bl UiWork_Finalize
	movs r1, #2
	mov r0, r10
	bl UiWork_Finalize
	bl Scheduler_ScheduleCallbackA
	movs r0, #55
	bl Runtime_ReleaseHeapBlock
	ldr r3, .L_080207a8
	ldr r1, .L_080207ac
	adds r2, r3, r1
	ldrb r0, [r2]
	ldr r2, .L_080207b0
	adds r3, r3, r2
	ldrb r1, [r3]
	bl PaletteGlow_Update
	movs r0, #1
	bl WaitFrames
.L_08020794:
	adds r0, r5, #0
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_080207a8:
	.4byte gCell
.L_080207ac:
	.4byte 0x00000205
.L_080207b0:
	.4byte 0x00000206
.L_080207b4:
	.4byte gKeysRepeat
.L_080207b8:
	.4byte 0x0000105c
.L_080207bc:
	.4byte 0x00001070
.L_080207c0:
	.4byte gKeyState
