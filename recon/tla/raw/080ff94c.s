.syntax unified
	.thumb
	.global PsynergyMenu_DrawRangePage
	.thumb_func
PsynergyMenu_DrawRangePage:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	mov r11, r2
	ldr r3, [r3]
	ldr r2, [r2, #8]
	mov r1, r11
	mov r8, r3
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r2, [r1, #16]
	adds r7, r0, #0
	adds r3, r3, r2
	mov r2, r8
	str r3, [r1, #24]
	ldr r0, [r2, #48]
	sub sp, #8
	bl RenderOutput_RedrawSavedRectFar
	movs r0, #1
	bl WaitFrames
	mov r1, r11
	ldr r3, [r1, #24]
	movs r2, #226
	lsls r2, r2, #1
	lsls r3, r3, #1
	adds r3, r3, r2
	mov r1, r8
	ldrh r2, [r1, r3]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080ffa3a
	movs r5, #252
	lsls r5, r5, #6
	ldr r3, .L_080ffb4c
	adds r5, #255
	adds r0, r5, #0
	ands r0, r2
	adds r0, r0, r3
	ldr r1, [r1, #48]
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	mov r2, r11
	ldr r3, [r2, #24]
	movs r1, #226
	lsls r1, r1, #1
	lsls r3, r3, #1
	adds r3, r3, r1
	mov r2, r8
	ldrh r3, [r2, r3]
	ands r5, r3
	adds r0, r5, #0
	bl BattleAction_Get
	movs r3, #104
	adds r5, r0, #0
	str r3, [sp, #0]
	adds r0, r7, #0
	movs r3, #224
	movs r1, #0
	movs r2, #96
	bl UiWindow_ClearInteriorTilesFar
	movs r3, #0
	mov r10, r3
	ldrb r3, [r5, #6]
	cmp r3, #0
	bne .L_080ff9f4
	ldrb r0, [r5, #1]
	movs r3, #64
	ands r3, r0
	cmp r3, #0
	beq .L_080ff9fa
	b .L_080ff9f6
.L_080ff9f4:
	ldrb r0, [r5, #1]
.L_080ff9f6:
	movs r1, #2
	mov r10, r1
.L_080ff9fa:
	movs r3, #128
	ands r3, r0
	cmp r3, #0
	beq .L_080ffa0a
	mov r2, r10
	movs r3, #1
	orrs r2, r3
	mov r10, r2
.L_080ffa0a:
	mov r3, r10
	cmp r3, #3
	bne .L_080ffa14
	ldr r0, .L_080ffb50
	b .L_080ffa1c
.L_080ffa14:
	mov r1, r10
	cmp r1, #2
	bne .L_080ffa28
	ldr r0, .L_080ffb54
.L_080ffa1c:
	adds r1, r7, #0
	movs r2, #0
	movs r3, #96
	bl UiText_DrawCharacterAtOffsetFar
	b .L_080ffa3a
.L_080ffa28:
	mov r2, r10
	cmp r2, #1
	bne .L_080ffa3a
	ldr r0, .L_080ffb58
	adds r1, r7, #0
	movs r2, #0
	movs r3, #96
	bl UiText_DrawCharacterAtOffsetFar
.L_080ffa3a:
	mov r3, r11
	ldr r2, [r3, #8]
	movs r1, #0
	lsls r3, r2, #2
	adds r3, r3, r2
	mov r10, r1
	lsls r3, r3, #1
	movs r1, #226
	add r3, r8
	lsls r1, r1, #1
	movs r2, #1
	adds r1, r1, r3
	mov r9, r2
	movs r6, #2
	mov r8, r1
.L_080ffa58:
	mov r2, r11
	ldr r3, [r2, #16]
	cmp r10, r3
	bne .L_080ffabe
	mov r1, r8
	ldrh r3, [r1]
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	ands r0, r3
	bl BattleAction_Get
	adds r5, r0, #0
	ldrb r3, [r5, #2]
	cmp r3, #4
	beq .L_080ffaa8
	adds r1, r3, #0
	movs r3, #0
	str r3, [sp, #0]
	adds r1, #1
	adds r0, r7, #0
	movs r2, #24
	adds r3, r6, #0
	bl UiWindow_SetTilemapEntryFar
	mov r2, r9
	movs r3, #14
	str r2, [sp, #0]
	str r3, [sp, #4]
	adds r0, r7, #0
	movs r1, #9
	adds r2, r6, #0
	movs r3, #15
	bl Func_080f9224
	mov r1, r9
	movs r2, #14
	str r1, [sp, #0]
	str r2, [sp, #4]
	b .L_080ffb04
.L_080ffaa8:
	mov r3, r9
	movs r1, #14
	str r3, [sp, #0]
	str r1, [sp, #4]
	adds r0, r7, #0
	movs r1, #9
	adds r2, r6, #0
	movs r3, #19
	bl Func_080f9224
	b .L_080ffb26
.L_080ffabe:
	mov r2, r8
	ldrh r3, [r2]
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	ands r0, r3
	bl BattleAction_Get
	adds r5, r0, #0
	ldrb r3, [r5, #2]
	cmp r3, #4
	beq .L_080ffb12
	adds r1, r3, #0
	movs r3, #4
	str r3, [sp, #0]
	adds r1, #1
	adds r0, r7, #0
	movs r2, #24
	adds r3, r6, #0
	bl UiWindow_SetTilemapEntryFar
	mov r3, r9
	movs r1, #15
	str r3, [sp, #0]
	str r1, [sp, #4]
	adds r0, r7, #0
	movs r1, #9
	adds r2, r6, #0
	movs r3, #15
	bl Func_080f9224
	mov r2, r9
	movs r3, #15
	str r2, [sp, #0]
	str r3, [sp, #4]
.L_080ffb04:
	adds r0, r7, #0
	movs r1, #25
	adds r2, r6, #0
	movs r3, #3
	bl Func_080f9224
	b .L_080ffb26
.L_080ffb12:
	mov r1, r9
	movs r2, #15
	str r1, [sp, #0]
	str r2, [sp, #4]
	adds r0, r7, #0
	movs r1, #9
	adds r2, r6, #0
	movs r3, #19
	bl Func_080f9224
.L_080ffb26:
	movs r1, #1
	add r10, r1
	movs r3, #2
	mov r2, r10
	adds r6, #2
	add r8, r3
	cmp r2, #4
	ble .L_080ffa58
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080ffb4c:
	.4byte 0x00000885
.L_080ffb50:
	.4byte 0x00001044
.L_080ffb54:
	.4byte 0x00001043
.L_080ffb58:
	.4byte 0x00001042
