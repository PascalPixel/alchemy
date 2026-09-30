.syntax unified
	.thumb
	.global CharacterMenu_DrawStatusAilments
	.thumb_func
CharacterMenu_DrawStatusAilments:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #40
	str r2, [sp, #24]
	str r1, [sp, #28]
	ldr r3, .L_080a88d8
	adds r7, r0, #0
	ldr r3, [r3]
	movs r0, #1
	negs r0, r0
	mov r8, r3
	bl Party_SumDjinnCountsFar
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	ldr r0, [sp, #28]
	str r3, [sp, #12]
	bl Func_08077008
	ldr r2, [sp, #24]
	movs r3, #255
	ands r3, r2
	movs r2, #7
	str r0, [sp, #20]
	str r2, [sp, #16]
	cmp r3, #1
	beq .L_080a864a
	movs r3, #10
	str r3, [sp, #16]
.L_080a864a:
	movs r3, #190
	lsls r3, r3, #1
	add r3, r8
	ldr r2, [r3]
	movs r3, #1
	strb r3, [r2, #5]
	ldr r1, [sp, #28]
	ldr r2, [sp, #24]
	adds r0, r7, #0
	bl ItemMenu_DrawOwnerStatus
	add r5, sp, #32
	ldr r2, [sp, #28]
	movs r1, #1
	adds r0, r5, #0
	bl CharacterMenu_BuildAvailability
	adds r0, r5, #0
	bl CharacterMenu_UpdateSelectionIcons
	movs r6, #128
	ldr r2, [sp, #24]
	lsls r6, r6, #1
	ands r6, r2
	cmp r6, #0
	bne .L_080a868c
	movs r3, #96
	adds r0, r7, #0
	movs r1, #0
	movs r2, #40
	str r3, [sp, #0]
	bl UiWindow_ClearInteriorTilesFar
.L_080a868c:
	movs r3, #0
	mov r10, r3
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_080a86a6
	movs r2, #16
	ldr r0, .L_080a88dc
	adds r1, r7, #0
	movs r3, #40
	bl UiText_DrawCharacterAtOffsetFar
	movs r2, #1
	mov r10, r2
.L_080a86a6:
	ldrb r3, [r5, #1]
	cmp r3, #0
	beq .L_080a86c0
	mov r2, r10
	lsls r3, r2, #4
	adds r3, #40
	ldr r0, .L_080a88e0
	adds r1, r7, #0
	movs r2, #16
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #1
	add r10, r3
.L_080a86c0:
	ldrb r3, [r5, #2]
	cmp r3, #0
	beq .L_080a86da
	mov r2, r10
	lsls r3, r2, #4
	adds r3, #40
	ldr r0, .L_080a88e4
	adds r1, r7, #0
	movs r2, #16
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #1
	add r10, r3
.L_080a86da:
	ldrb r3, [r5, #3]
	cmp r3, #0
	beq .L_080a86f4
	mov r2, r10
	lsls r3, r2, #4
	adds r3, #40
	ldr r0, .L_080a88e8
	adds r1, r7, #0
	movs r2, #16
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #1
	add r10, r3
.L_080a86f4:
	ldrb r3, [r5, #4]
	cmp r3, #0
	beq .L_080a870e
	mov r2, r10
	lsls r3, r2, #4
	adds r3, #40
	ldr r0, .L_080a88ec
	adds r1, r7, #0
	movs r2, #16
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #1
	add r10, r3
.L_080a870e:
	mov r2, r10
	cmp r2, #0
	bne .L_080a8720
	ldr r0, .L_080a88f0
	adds r1, r7, #0
	movs r2, #0
	movs r3, #40
	bl UiText_DrawCharacterAtOffsetFar
.L_080a8720:
	adds r0, r5, #0
	bl CharacterMenu_UpdateSelectionIcons
	adds r0, r5, #0
	bl ItemMenu_ApplyFlags
	movs r3, #136
	lsls r3, r3, #2
	add r3, r8
	ldrh r3, [r3]
	cmp r3, #3
	bne .L_080a873a
	b .L_080a88c6
.L_080a873a:
	cmp r6, #0
	bne .L_080a8754
	movs r0, #1
	bl WaitFrames
	movs r3, #96
	str r3, [sp, #0]
	adds r0, r7, #0
	movs r1, #64
	movs r2, #56
	movs r3, #224
	bl UiWindow_ClearInteriorTilesFar
.L_080a8754:
	movs r0, #15
	bl UiWork_SetParamNibbleFar
	ldr r3, [sp, #24]
	cmp r3, #1
	beq .L_080a8766
	ldr r2, [sp, #12]
	cmp r2, #1
	bne .L_080a87a0
.L_080a8766:
	movs r5, #4
	ldr r3, [sp, #16]
	adds r0, r7, #0
	movs r1, #1
	movs r2, #15
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntryFar
	adds r0, r7, #0
	ldr r3, [sp, #16]
	movs r1, #2
	movs r2, #19
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntryFar
	adds r0, r7, #0
	movs r1, #3
	movs r2, #23
	ldr r3, [sp, #16]
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntryFar
	adds r0, r7, #0
	movs r1, #4
	movs r2, #27
	ldr r3, [sp, #16]
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntryFar
.L_080a87a0:
	ldr r3, [sp, #12]
	cmp r3, #0
	beq .L_080a87b8
	ldr r2, [sp, #16]
	lsls r6, r2, #3
	adds r3, r6, #0
	ldr r0, .L_080a88f4
	adds r3, #8
	adds r1, r7, #0
	movs r2, #64
	bl UiText_DrawCharacterAtOffsetFar
.L_080a87b8:
	ldr r3, [sp, #24]
	cmp r3, #1
	bne .L_080a87fc
	ldr r2, [sp, #12]
	cmp r2, #0
	bne .L_080a87ca
	ldr r3, [sp, #16]
	subs r3, #1
	str r3, [sp, #16]
.L_080a87ca:
	ldr r2, [sp, #16]
	lsls r6, r2, #3
	adds r3, r6, #0
	ldr r0, .L_080a88f8
	adds r3, #16
	adds r1, r7, #0
	movs r2, #64
	bl Func_08015090
	ldr r5, .L_080a88fc
	adds r3, r6, #0
	adds r0, r5, #0
	adds r3, #24
	adds r1, r7, #0
	movs r2, #64
	bl UiText_DrawCharacterAtOffsetFar
	adds r5, #1
	adds r3, r6, #0
	adds r3, #32
	adds r0, r5, #0
	adds r1, r7, #0
	movs r2, #64
	bl UiText_DrawCharacterAtOffsetFar
.L_080a87fc:
	ldr r2, [sp, #16]
	movs r3, #0
	lsls r2, r2, #3
	mov r10, r3
	ldr r3, [sp, #20]
	str r2, [sp, #8]
	adds r2, #8
	adds r3, #72
	str r2, [sp, #4]
	movs r2, #104
	mov r8, r3
	mov r11, r2
	movs r3, #120
	ldr r2, [sp, #20]
	mov r9, r3
	adds r3, #160
	adds r5, r2, r3
.L_080a881e:
	ldr r2, [sp, #12]
	cmp r2, #0
	beq .L_080a8834
	ldr r3, [sp, #4]
	ldrb r0, [r5]
	movs r1, #1
	str r3, [sp, #0]
	adds r2, r7, #0
	mov r3, r9
	bl UiNumber_DrawAt
.L_080a8834:
	ldr r2, [sp, #24]
	movs r3, #255
	ands r3, r2
	cmp r3, #1
	bne .L_080a88b0
	ldr r3, [sp, #12]
	cmp r3, #0
	beq .L_080a8866
	ldr r2, [sp, #4]
	ldrb r0, [r5, #4]
	movs r1, #1
	str r2, [sp, #0]
	mov r3, r11
	adds r2, r7, #0
	ldr r6, [sp, #8]
	bl UiNumber_DrawAt
	mov r2, r9
	subs r2, #8
	ldr r0, .L_080a8900
	adds r1, r7, #0
	ldr r3, [sp, #4]
	bl UiText_DrawStringInWindowFar
	b .L_080a886a
.L_080a8866:
	ldr r3, [sp, #16]
	lsls r6, r3, #3
.L_080a886a:
	ldr r0, [sp, #28]
	mov r1, r10
	bl Owner_GetResistanceValueFar
	adds r2, r6, #0
	adds r2, #16
	mov r3, r9
	str r2, [sp, #0]
	subs r3, #8
	adds r2, r7, #0
	movs r1, #2
	bl UiNumber_DrawAt
	mov r3, r8
	movs r2, #0
	ldrsh r0, [r3, r2]
	adds r3, r6, #0
	adds r3, #24
	adds r2, r7, #0
	str r3, [sp, #0]
	movs r1, #3
	mov r3, r11
	bl UiNumber_DrawAt
	mov r3, r8
	movs r2, #2
	ldrsh r0, [r3, r2]
	adds r3, r6, #0
	adds r3, #32
	str r3, [sp, #0]
	movs r1, #3
	adds r2, r7, #0
	mov r3, r11
	bl UiNumber_DrawAt
.L_080a88b0:
	movs r2, #4
	add r8, r2
	movs r2, #1
	movs r3, #32
	add r10, r2
	add r11, r3
	add r9, r3
	mov r3, r10
	adds r5, #1
	cmp r3, #3
	ble .L_080a881e
.L_080a88c6:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080a88d8:
	.4byte Data_03001f2c
.L_080a88dc:
	.4byte 0x00000bd5
.L_080a88e0:
	.4byte 0x00000bd6
.L_080a88e4:
	.4byte 0x00000bd7
.L_080a88e8:
	.4byte 0x00000bd8
.L_080a88ec:
	.4byte 0x00000bd9
.L_080a88f0:
	.4byte 0x00000bd4
.L_080a88f4:
	.4byte 0x00000afd
.L_080a88f8:
	.4byte Menu_LvString
.L_080a88fc:
	.4byte 0x00000afe
.L_080a8900:
	.4byte Data_080af230
