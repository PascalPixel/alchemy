.syntax unified
	.thumb
	.global Unnamed_080a5388
	.thumb_func
Unnamed_080a5388:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #0
	sub sp, #12
	mov r8, r3
	movs r3, #1
	str r3, [sp, #8]
	ldr r3, .L_080a5518
	ldr r3, [r3]
	ldr r6, .L_080a551c
	mov r9, r3
	add r6, r9
	ldrb r0, [r6]
	bl Owner_GetStateFar
	movs r3, #187
	str r0, [sp, #4]
	lsls r3, r3, #1
	add r3, r9
	ldrh r1, [r3]
	mov r10, r3
	ldrb r3, [r6]
	movs r5, #166
	adds r0, r3, #0
	movs r2, #0
	lsls r5, r5, #1
	bl ItemMenu_DrawEquipPreview
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	ldr r3, .L_080a5520
	ldr r1, [sp, #4]
	adds r2, r5, #0
	mov r11, r0
	bl _call_via_r3
	movs r3, #134
	lsls r3, r3, #1
	add r3, r9
	ldr r7, [r3]
	mov r3, r10
	ldrb r0, [r6]
	ldrh r1, [r3]
	bl Inventory_EquipFar
	adds r0, #2
	cmp r0, #1
	bhi .L_080a53fe
	b .L_080a54c6
.L_080a53f6:
	movs r0, #175
	bl AudioCommand_PlayFar
	b .L_080a54ca
.L_080a53fe:
	ldr r5, .L_080a5524
	adds r1, r7, #0
	adds r0, r5, #0
	movs r2, #24
	movs r3, #24
	adds r5, #1
	bl UiText_DrawCharacterAtOffsetFar
	adds r0, r5, #0
	adds r1, r7, #0
	movs r2, #72
	movs r3, #24
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #24
	str r3, [sp, #0]
	adds r0, r7, #0
	movs r1, #16
	movs r2, #16
	movs r3, #96
	bl UiWindow_ClearInteriorTilesFar
	adds r1, r7, #0
	ldr r0, .L_080a5528
	movs r2, #0
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
	movs r0, #110
	movs r1, #32
	bl UiMenu_SlideCursor
	b .L_080a5488
.L_080a5440:
	mov r3, r8
	lsls r0, r3, #1
	add r0, r8
	lsls r0, r0, #4
	adds r0, #110
	movs r1, #32
	bl UiMenu_PositionCursor
	ldr r5, .L_080a552c
	ldr r3, [r5]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_080a546c
	movs r3, #1
	negs r3, r3
	add r8, r3
	movs r0, #111
	movs r3, #1
	str r3, [sp, #8]
	bl AudioCommand_PlayFar
.L_080a546c:
	ldr r3, [r5]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_080a5482
	movs r3, #1
	movs r0, #111
	add r8, r3
	str r3, [sp, #8]
	bl AudioCommand_PlayFar
.L_080a5482:
	movs r0, #1
	bl WaitFrames
.L_080a5488:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_IsSet
	cmp r0, #0
	bne .L_080a54ca
	ldr r3, [sp, #8]
	cmp r3, #0
	beq .L_080a54aa
	mov r0, r8
	movs r3, #0
	adds r0, #2
	movs r1, #2
	str r3, [sp, #8]
	bl Math_Mod
	mov r8, r0
.L_080a54aa:
	ldr r1, .L_080a5530
	ldr r3, [r1]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_080a53f6
	ldr r3, [r1]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080a5440
	movs r0, #113
	bl AudioCommand_PlayFar
.L_080a54c6:
	movs r3, #1
	mov r8, r3
.L_080a54ca:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_IsSet
	cmp r0, #0
	beq .L_080a54da
	movs r3, #1
	mov r8, r3
.L_080a54da:
	mov r3, r8
	cmp r3, #1
	bne .L_080a54ee
	movs r2, #166
	ldr r3, .L_080a5520
	ldr r0, [sp, #4]
	mov r1, r11
	lsls r2, r2, #1
	bl _call_via_r3
.L_080a54ee:
	ldr r5, .L_080a551c
	mov r0, r11
	add r5, r9
	bl Runtime_BumpFree
	ldrb r0, [r5]
	bl Owner_RecalculateStatsFar
	ldrb r0, [r5]
	bl Owner_RefreshClassActionsFar
	mov r0, r8
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_080a5518:
	.4byte gMenuWork
.L_080a551c:
	.4byte 0x0000021b
.L_080a5520:
	.4byte IwramCopyWords
.L_080a5524:
	.4byte 0x00000b2c
.L_080a5528:
	.4byte 0x00000ad6
.L_080a552c:
	.4byte gKeysRepeat
.L_080a5530:
	.4byte gKeyState
