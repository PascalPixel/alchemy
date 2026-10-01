.syntax unified
	.thumb
	.global Func_080a4f08
Func_080a4f08:
	.global ItemMenu_SelectGiveQuantity
	.thumb_func
ItemMenu_SelectGiveQuantity:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #28
	str r1, [sp, #24]
	str r2, [sp, #20]
	ldr r3, .L_080a4fdc
	movs r1, #128
	ldr r3, [r3]
	lsls r1, r1, #3
	mov r11, r0
	movs r0, #14
	mov r9, r3
	bl Runtime_AllocateBlock
	movs r3, #0
	movs r2, #1
	str r3, [sp, #8]
	movs r3, #134
	str r2, [sp, #12]
	lsls r3, r3, #1
	add r3, r9
	ldr r7, [r3]
	mov r10, r0
	bl ItemMenu_SetMsgWin7
	adds r0, r7, #0
	bl RenderOutput_RedrawSavedRectFar
	ldr r2, [sp, #20]
	mov r8, r11
	cmp r2, #0
	bne .L_080a4f6a
	ldr r3, .L_080a4fe0
	add r3, r9
	ldrb r0, [r3]
	movs r3, #188
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	ldr r1, .L_080a4fe4
	ands r1, r3
	bl InventoryMenu_GetItemQuantity
	str r0, [sp, #8]
.L_080a4f6a:
	ldr r3, .L_080a4fe8
	add r3, r9
	ldrb r0, [r3]
	movs r3, #188
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	ldr r1, .L_080a4fe4
	ands r1, r3
	bl InventoryMenu_GetItemQuantity
	str r0, [sp, #4]
	bl Resource_FindFreeEntry
	str r0, [sp, #16]
	cmp r0, #96
	bne .L_080a4f8e
	b .L_080a517c
.L_080a4f8e:
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	bl VramBlock_LoadCached
	ldr r6, .L_080a4fec
	movs r5, #32
	ldr r0, [sp, #16]
	adds r1, r6, #0
	adds r2, r7, #0
	movs r3, #48
	str r5, [sp, #0]
	bl RenderOutput_CreateFar
	adds r1, r6, #0
	adds r2, r7, #0
	ldr r0, [sp, #16]
	movs r3, #80
	str r5, [sp, #0]
	bl RenderOutput_CreateFar
	ldrh r1, [r0, #24]
	lsls r2, r1, #22
	ldr r3, .L_080a4fd8
	lsrs r2, r2, #22
	adds r2, #4
	ands r2, r3
	ldr r3, .L_080a4ff0
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #24]
	movs r1, #40
	movs r0, #128
	bl UiMenu_SlideCursor
	b .L_080a516e
	.2byte 0x0000
.L_080a4fd8:
	.4byte 0x000003ff
.L_080a4fdc:
	.4byte gMenuWork
.L_080a4fe0:
	.4byte 0x0000021b
.L_080a4fe4:
	.4byte 0x000001ff
.L_080a4fe8:
	.4byte 0x0000021a
.L_080a4fec:
	.4byte 0x40004000
.L_080a4ff0:
	.4byte 0xfffffc00
.L_080a4ff4:
	ldr r3, [sp, #12]
	cmp r3, #0
	bne .L_080a4ffc
	b .L_080a5104
.L_080a4ffc:
	ldr r0, [sp, #24]
	ldr r1, [sp, #24]
	movs r2, #0
	add r0, r8
	str r2, [sp, #12]
	bl __modsi3
	mov r8, r0
	adds r0, r7, #0
	bl RenderOutput_RedrawSavedRectFar
	adds r1, r7, #0
	ldr r0, .L_080a50b0
	movs r2, #32
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r3, .L_080a50b4
	ldr r0, .L_080a50b8
	mov r1, r10
	ldr r2, .L_080a50bc
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r2, r10
	movs r0, #30
	movs r1, #14
	bl Shop_FillSelectorFar
	ldr r0, [sp, #24]
	movs r1, #0
	add r0, r11
	mov r2, r10
	bl Shop_FillSelectorFar
	mov r0, r11
	add r0, r8
	adds r0, #1
	movs r1, #10
	mov r2, r10
	bl Shop_FillSelectorFar
	mov r0, r11
	movs r1, #2
	mov r2, r10
	bl Shop_FillSelectorFar
	movs r1, #128
	ldr r0, [sp, #16]
	lsls r1, r1, #1
	mov r2, r10
	bl VramBlock_LoadCached
	mov r0, r8
	movs r3, #32
	adds r0, #1
	movs r1, #2
	adds r2, r7, #0
	str r3, [sp, #0]
	bl UiNumber_DrawAt
	movs r3, #188
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	ldr r0, .L_080a50ac
	ands r0, r3
	ldr r3, .L_080a50c0
	adds r1, r7, #0
	adds r0, r0, r3
	movs r2, #16
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	ldr r3, [sp, #4]
	mov r2, r8
	subs r0, r3, r2
	subs r0, #1
	movs r3, #16
	movs r5, #24
	movs r1, #2
	adds r2, r7, #0
	str r5, [sp, #0]
	bl UiNumber_DrawAt
	ldr r3, [sp, #20]
	cmp r3, #0
	bne .L_080a50d6
	b .L_080a50c4
.L_080a50ac:
	.4byte 0x000001ff
.L_080a50b0:
	.4byte 0x00000ade
.L_080a50b4:
	.4byte 0x040000d4
.L_080a50b8:
	.4byte Data_080af08c
.L_080a50bc:
	.4byte 0x84000040
.L_080a50c0:
	.4byte 0x00000182
.L_080a50c4:
	ldr r0, [sp, #8]
	add r0, r8
	adds r0, #1
	movs r1, #2
	adds r2, r7, #0
	movs r3, #80
	str r5, [sp, #0]
	bl UiNumber_DrawAt
.L_080a50d6:
	ldr r3, .L_080a51c0
	add r3, r9
	ldrb r0, [r3]
	bl Owner_GetStateFar
	movs r2, #16
	adds r1, r7, #0
	movs r3, #16
	bl UiText_DrawStringAtOffsetFar
	ldr r2, [sp, #20]
	cmp r2, #0
	bne .L_080a5104
	ldr r3, .L_080a51c4
	add r3, r9
	ldrb r0, [r3]
	bl Owner_GetStateFar
	adds r1, r7, #0
	movs r2, #80
	movs r3, #16
	bl UiText_DrawStringAtOffsetFar
.L_080a5104:
	ldr r1, .L_080a51c8
	ldr r3, [r1]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080a5118
	movs r0, #112
	bl AudioCommand_PlayFar
	b .L_080a517c
.L_080a5118:
	ldr r3, [r1]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080a5130
	movs r3, #1
	negs r3, r3
	movs r0, #113
	mov r8, r3
	bl AudioCommand_PlayFar
	b .L_080a517c
.L_080a5130:
	movs r0, #128
	movs r1, #40
	bl UiMenu_PositionCursor
	ldr r5, .L_080a51cc
	ldr r3, [r5]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_080a5152
	subs r2, #33
	movs r3, #1
	movs r0, #111
	add r8, r2
	str r3, [sp, #12]
	bl AudioCommand_PlayFar
.L_080a5152:
	ldr r3, [r5]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_080a5168
	movs r2, #1
	movs r0, #111
	add r8, r2
	str r2, [sp, #12]
	bl AudioCommand_PlayFar
.L_080a5168:
	movs r0, #1
	bl WaitFrames
.L_080a516e:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_IsSet
	cmp r0, #0
	bne .L_080a517c
	b .L_080a4ff4
.L_080a517c:
	adds r0, r7, #0
	bl RenderOutput_RedrawSavedRectFar
	adds r0, r7, #0
	bl RenderOutput_ClearListFar
	movs r0, #14
	bl Runtime_ReleaseHeapBlock
	movs r3, #135
	lsls r3, r3, #2
	add r3, r9
	ldr r2, [r3]
	movs r0, #168
	movs r3, #13
	strb r3, [r2, #5]
	lsls r0, r0, #1
	bl GameFlag_IsSet
	cmp r0, #0
	beq .L_080a51ac
	movs r3, #1
	negs r3, r3
	mov r8, r3
.L_080a51ac:
	mov r0, r8
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_080a51c0:
	.4byte 0x0000021a
.L_080a51c4:
	.4byte 0x0000021b
.L_080a51c8:
	.4byte gKeyState
.L_080a51cc:
	.4byte gKeysRepeat
