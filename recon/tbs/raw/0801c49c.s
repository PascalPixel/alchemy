.syntax unified
	.thumb
	.global Menu_Check
	.thumb_func
Menu_Check:
	.global Debug_SelectAbilityPair
	.thumb_func
Debug_SelectAbilityPair:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #20
	movs r0, #0
	str r0, [sp, #4]
	mov r8, r0
	movs r0, #224
	movs r1, #1
	lsls r0, r0, #3
	mov r11, r1
	bl Runtime_BumpAllocate
	movs r1, #140
	adds r7, r0, #0
	movs r0, #0
	bl OwnerAction_AddFar
	movs r1, #140
	movs r0, #1
	bl OwnerAction_AddFar
	movs r1, #140
	movs r0, #2
	bl OwnerAction_AddFar
	movs r1, #141
	movs r0, #2
	bl OwnerAction_AddFar
	movs r1, #78
	movs r0, #2
	bl OwnerAction_AddFar
	movs r1, #93
	movs r0, #3
	bl OwnerAction_AddFar
	movs r1, #140
	movs r0, #5
	bl OwnerAction_AddFar
	movs r3, #0
	adds r0, r7, #0
	str r3, [sp, #12]
	str r3, [sp, #16]
	bl Object_CollectResources
	mov r10, r0
	cmp r0, #0
	bne .L_0801c50c
	b .L_0801c7b6
.L_0801c50c:
	add r0, sp, #16
	add r1, sp, #12
	adds r2, r7, #0
	bl Menu_FindShortcutEntries
	movs r5, #2
	movs r1, #6
	movs r2, #20
	movs r3, #7
	movs r0, #4
	str r5, [sp, #0]
	bl UiWindow_Create
	movs r1, #3
	movs r2, #20
	movs r3, #3
	adds r6, r0, #0
	movs r0, #4
	str r5, [sp, #0]
	bl UiWindow_Create
	movs r1, #14
	str r0, [sp, #8]
	movs r2, #20
	movs r3, #5
	movs r0, #4
	str r5, [sp, #0]
	bl UiWindow_Create
	mov r9, r0
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0801c56e
	ldr r2, .L_0801c7d0
	movs r1, #128
	bl VramBlock_LoadCached
	movs r0, #0
	movs r1, #128
	str r0, [sp, #0]
	lsls r1, r1, #23
	adds r0, r5, #0
	adds r2, r6, #0
	movs r3, #0
	bl RenderOutput_Create
	str r0, [sp, #4]
.L_0801c56e:
	ldr r5, .L_0801c7d4
	ldr r1, [sp, #8]
	adds r0, r5, #0
	movs r2, #16
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
.L_0801c57c:
	mov r1, r11
	cmp r1, #0
	bne .L_0801c584
	b .L_0801c6a8
.L_0801c584:
	ldr r0, [sp, #16]
	movs r3, #0
	mov r1, r10
	add r0, r10
	mov r11, r3
	bl IwramUnsignedRemainderEntry
	str r0, [sp, #16]
	ldr r0, [sp, #12]
	mov r1, r10
	add r0, r10
	bl IwramUnsignedRemainderEntry
	mov r2, r8
	adds r2, #2
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	lsls r3, r3, #1
	subs r2, r2, r3
	ldrh r3, [r6, #14]
	mov r8, r2
	lsls r3, r3, #3
	lsls r2, r2, #4
	str r0, [sp, #12]
	adds r2, r2, r3
	ldr r0, [sp, #4]
	adds r2, #28
	strh r2, [r0, #8]
	strb r2, [r0, #20]
	adds r0, r6, #0
	bl RenderOutput_RedrawSavedRect
	movs r3, #2
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r1, #1
	movs r2, #2
	movs r3, #17
	bl UiWindow_DrawDividerLine
	ldr r0, .L_0801c7d8
	adds r1, r6, #0
	movs r2, #48
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	ldr r3, [sp, #16]
	lsls r3, r3, #2
	adds r3, r3, r7
	ldr r5, .L_0801c7dc
	ldrh r0, [r3, #2]
	adds r1, r6, #0
	adds r0, r0, r5
	movs r2, #56
	movs r3, #16
	bl UiText_DrawCharacterAtOffset
	ldr r3, [sp, #12]
	lsls r3, r3, #2
	adds r3, r3, r7
	ldrh r0, [r3, #2]
	adds r1, r6, #0
	adds r0, r0, r5
	movs r2, #56
	movs r3, #32
	bl UiText_DrawCharacterAtOffset
	ldr r0, .L_0801c7d8
	adds r1, r6, #0
	subs r0, #2
	movs r2, #16
	movs r3, #16
	bl UiText_DrawCharacterAtOffset
	ldr r0, .L_0801c7d8
	adds r1, r6, #0
	subs r0, #1
	movs r2, #16
	movs r3, #32
	bl UiText_DrawCharacterAtOffset
	ldr r3, [sp, #16]
	lsls r3, r3, #2
	ldrh r0, [r3, r7]
	ldr r5, .L_0801c7e0
	adds r1, r6, #0
	adds r0, r0, r5
	movs r2, #104
	movs r3, #16
	bl UiText_DrawCharacterAtOffset
	ldr r3, [sp, #12]
	lsls r3, r3, #2
	ldrh r0, [r3, r7]
	adds r1, r6, #0
	movs r2, #104
	movs r3, #32
	adds r0, r0, r5
	bl UiText_DrawCharacterAtOffset
	mov r0, r9
	bl RenderOutput_RedrawSavedRect
	mov r1, r9
	ldr r0, .L_0801c7e4
	movs r2, #0
	movs r3, #16
	bl UiText_DrawCharacterAtOffset
	mov r1, r8
	cmp r1, #0
	beq .L_0801c676
	ldr r3, [sp, #12]
	lsls r3, r3, #2
	adds r3, r3, r7
	ldrh r0, [r3, #2]
	bl Func_08077080
	ldr r3, [sp, #12]
	b .L_0801c684
.L_0801c676:
	ldr r3, [sp, #16]
	lsls r3, r3, #2
	adds r3, r3, r7
	ldrh r0, [r3, #2]
	bl Func_08077080
	ldr r3, [sp, #16]
.L_0801c684:
	lsls r3, r3, #2
	adds r3, r3, r7
	ldrb r0, [r0, #9]
	ldrh r5, [r3, #2]
	movs r3, #16
	str r3, [sp, #0]
	movs r1, #2
	mov r2, r9
	movs r3, #64
	bl UiText_DrawNumberAtOffset
	ldr r0, .L_0801c7e8
	mov r1, r9
	adds r0, r5, r0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
.L_0801c6a8:
	movs r0, #1
	bl WaitFrames
	ldr r5, .L_0801c7ec
	ldr r3, [r5]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_0801c6da
	movs r0, #111
	bl Func_080f9010
	mov r3, r8
	cmp r3, #0
	beq .L_0801c6ce
	ldr r3, [sp, #12]
	subs r3, #1
	str r3, [sp, #12]
	b .L_0801c6d4
.L_0801c6ce:
	ldr r3, [sp, #16]
	subs r3, #1
	str r3, [sp, #16]
.L_0801c6d4:
	movs r0, #1
	ldr r5, .L_0801c7ec
	mov r11, r0
.L_0801c6da:
	ldr r3, [r5]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_0801c704
	movs r0, #111
	bl Func_080f9010
	mov r1, r8
	cmp r1, #0
	beq .L_0801c6f8
	ldr r3, [sp, #12]
	adds r3, #1
	str r3, [sp, #12]
	b .L_0801c6fe
.L_0801c6f8:
	ldr r3, [sp, #16]
	adds r3, #1
	str r3, [sp, #16]
.L_0801c6fe:
	movs r3, #1
	ldr r5, .L_0801c7ec
	mov r11, r3
.L_0801c704:
	ldr r3, [r5]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_0801c71e
	movs r0, #111
	bl Func_080f9010
	movs r0, #1
	negs r0, r0
	movs r1, #1
	add r8, r0
	mov r11, r1
.L_0801c71e:
	ldr r3, [r5]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_0801c734
	movs r0, #111
	bl Func_080f9010
	movs r3, #1
	add r8, r3
	mov r11, r3
.L_0801c734:
	ldr r1, .L_0801c7f0
	ldr r3, [r1]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0801c748
	movs r0, #112
	bl Func_080f9010
	b .L_0801c76c
.L_0801c748:
	ldr r3, [r1]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0801c75a
	movs r0, #113
	bl Func_080f9010
	b .L_0801c76c
.L_0801c75a:
	ldr r3, [r1]
	movs r2, #8
	ands r3, r2
	cmp r3, #0
	bne .L_0801c766
	b .L_0801c57c
.L_0801c766:
	movs r0, #113
	bl Func_080f9010
.L_0801c76c:
	ldr r3, [sp, #16]
	lsls r3, r3, #2
	adds r3, r3, r7
	ldrh r2, [r3]
	ldr r1, .L_0801c7f4
	ldrh r3, [r3, #2]
	movs r0, #136
	lsls r0, r0, #2
	lsls r2, r2, #10
	orrs r2, r3
	adds r3, r1, r0
	strh r2, [r3]
	ldr r3, [sp, #12]
	lsls r3, r3, #2
	adds r3, r3, r7
	ldrh r2, [r3]
	ldrh r3, [r3, #2]
	lsls r2, r2, #10
	orrs r2, r3
	ldr r3, .L_0801c7f8
	adds r1, r1, r3
	strh r2, [r1]
	adds r0, r6, #0
	movs r1, #1
	bl UiWork_Finalize
	movs r1, #1
	ldr r0, [sp, #8]
	bl UiWork_Finalize
	mov r0, r9
	movs r1, #1
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
.L_0801c7b6:
	adds r0, r7, #0
	bl Runtime_BumpFree
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_0801c7d0:
	.4byte Resource_FixedBlockBTiles
.L_0801c7d4:
	.4byte 0x00000b19
.L_0801c7d8:
	.4byte 0x00000b1e
.L_0801c7dc:
	.4byte 0x00000333
.L_0801c7e0:
	.4byte 0x00000066
.L_0801c7e4:
	.4byte 0x00000aec
.L_0801c7e8:
	.4byte 0x0000053a
.L_0801c7ec:
	.4byte gKeysRepeat
.L_0801c7f0:
	.4byte gKeyState
.L_0801c7f4:
	.4byte gCell
.L_0801c7f8:
	.4byte 0x00000222
