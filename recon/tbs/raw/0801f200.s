.syntax unified
	.thumb
	.global UiWindow_DrawPartyStatusContents
	.thumb_func
UiWindow_DrawPartyStatusContents:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0801f2bc
	ldr r6, [r3]
	adds r2, r3, #0
	subs r3, #4
	ldr r3, [r3]
	sub sp, #52
	subs r2, #28
	movs r5, #0
	ldr r1, .L_0801f2c0
	ldr r7, [r2]
	mov r11, r0
	ldr r0, [r6]
	str r3, [sp, #32]
	str r5, [sp, #24]
	adds r3, r3, r1
	ldrb r3, [r3]
	mov r8, r0
	cmp r3, #0
	beq .L_0801f288
	movs r0, #0
	bl BattleParty_PrepareActiveOwnersFar
	movs r2, #1
	movs r3, #0
	negs r2, r2
	mov r10, r3
	str r0, [sp, #28]
	str r2, [sp, #24]
	cmp r10, r0
	bcs .L_0801f2d8
	mov r4, sp
	adds r4, #40
	str r4, [sp, #16]
	movs r3, #88
	ldrh r3, [r7, r3]
	adds r0, r4, #0
	movs r1, #255
	strh r3, [r0]
	lsls r1, r1, #16
	lsls r3, r3, #16
	cmp r3, r1
	beq .L_0801f2d8
	adds r2, r7, #0
	ldr r0, [sp, #16]
	adds r2, #88
	movs r1, #0
.L_0801f26a:
	movs r3, #1
	ldr r4, [sp, #28]
	add r10, r3
	adds r1, #2
	cmp r10, r4
	bcs .L_0801f2d8
	adds r2, #2
	ldrh r3, [r2]
	movs r4, #255
	strh r3, [r1, r0]
	lsls r4, r4, #16
	lsls r3, r3, #16
	cmp r3, r4
	bne .L_0801f26a
	b .L_0801f2d8
.L_0801f288:
	bl Party_CountActiveOwnersFar
	str r0, [sp, #28]
	ldr r1, [sp, #28]
	movs r0, #0
	mov r10, r0
	cmp r10, r1
	bcs .L_0801f2c8
	mov r2, sp
	ldr r3, .L_0801f2c4
	movs r4, #252
	adds r2, #40
	lsls r4, r4, #1
	str r2, [sp, #16]
	adds r1, r2, #0
	adds r2, r3, r4
.L_0801f2a8:
	ldrb r3, [r2]
	strh r3, [r1]
	movs r0, #1
	ldr r3, [sp, #28]
	add r10, r0
	adds r2, #1
	adds r1, #2
	cmp r10, r3
	bcc .L_0801f2a8
	b .L_0801f2ce
.L_0801f2bc:
	.4byte Data_03001e90
.L_0801f2c0:
	.4byte 0x00000ea5
.L_0801f2c4:
	.4byte gCell
.L_0801f2c8:
	mov r4, sp
	adds r4, #40
	str r4, [sp, #16]
.L_0801f2ce:
	mov r0, r10
	ldr r3, .L_0801f304
	ldr r1, [sp, #16]
	lsls r2, r0, #1
	strh r3, [r1, r2]
.L_0801f2d8:
	movs r3, #1
	mov r2, r10
	negs r3, r3
	str r2, [sp, #28]
	cmp r11, r3
	bne .L_0801f2e8
	ldrh r4, [r6, #12]
	mov r11, r4
.L_0801f2e8:
	movs r3, #1
	mov r0, r11
	ands r3, r0
	cmp r3, #0
	bne .L_0801f2fa
	movs r3, #3
	negs r3, r3
	ands r0, r3
	mov r11, r0
.L_0801f2fa:
	ldr r1, [sp, #32]
	ldr r2, .L_0801f308
	adds r3, r1, r2
	ldrb r3, [r3]
	b .L_0801f30c
.L_0801f304:
	.4byte 0x000000ff
.L_0801f308:
	.4byte 0x00000ea5
.L_0801f30c:
	cmp r3, #0
	beq .L_0801f31c
	movs r0, #0
	movs r1, #0
	bl BattlePlacement_CountValidEntriesFar
	cmp r0, #0
	bne .L_0801f326
.L_0801f31c:
	movs r3, #3
	mov r4, r11
	negs r3, r3
	ands r4, r3
	mov r11, r4
.L_0801f326:
	mov r0, r11
	cmp r0, #9
	bne .L_0801f33a
	ldrh r0, [r6, #4]
	ldrh r1, [r6, #6]
	ldrh r2, [r6, #8]
	ldrh r3, [r6, #10]
	bl UiWindow_EraseBorderRect
	b .L_0801f5a6
.L_0801f33a:
	ldr r1, [sp, #32]
	ldr r2, .L_0801f5b8
	adds r3, r1, r2
	movs r2, #1
	strb r2, [r3]
	ldrh r3, [r6, #12]
	cmp r3, r11
	bne .L_0801f35a
	mov r0, r8
	bl RenderOutput_RedrawSavedRect
	mov r0, r8
	mov r1, r11
	bl UiWindow_DrawColumnBorders
	b .L_0801f392
.L_0801f35a:
	ldrh r1, [r6, #6]
	ldrh r2, [r6, #8]
	ldrh r3, [r6, #10]
	ldrh r0, [r6, #4]
	bl UiWindow_EraseBorderRect
	mov r0, r11
	bl UiWindow_BuildLayoutBounds
	ldrh r3, [r6, #8]
	mov r4, r8
	strh r3, [r4, #8]
	ldrh r3, [r6, #10]
	mov r0, r8
	strh r3, [r0, #10]
	ldrh r3, [r6, #4]
	mov r1, r8
	strh r3, [r1, #12]
	ldrh r0, [r6, #4]
	ldrh r1, [r6, #6]
	ldrh r2, [r6, #8]
	ldrh r3, [r6, #10]
	bl UiWindow_DrawFrame
	mov r0, r8
	mov r1, r11
	bl UiWindow_DrawColumnBorders
.L_0801f392:
	movs r3, #2
	mov r2, r11
	ands r3, r2
	cmp r3, #0
	beq .L_0801f39e
	movs r5, #5
.L_0801f39e:
	ldr r4, [sp, #28]
	movs r3, #0
	mov r10, r3
	cmp r4, #0
	bne .L_0801f3aa
	b .L_0801f4ea
.L_0801f3aa:
	ldr r1, [sp, #24]
	mov r0, sp
	lsls r1, r1, #3
	adds r2, r5, #1
	adds r0, #40
	lsls r5, r5, #3
	str r0, [sp, #16]
	str r1, [sp, #12]
	str r1, [sp, #20]
	str r2, [sp, #8]
	str r3, [sp, #4]
	mov r9, r5
.L_0801f3c2:
	ldr r3, [sp, #4]
	ldr r4, [sp, #16]
	ldrh r0, [r3, r4]
	bl Owner_GetStateFar
	adds r5, r0, #0
	movs r0, #56
	ldrsh r7, [r5, r0]
	movs r1, #52
	ldrsh r3, [r5, r1]
	cmp r7, #0
	bne .L_0801f3e2
	movs r0, #2
	bl UiWork_SetParamNibble
	b .L_0801f3fc
.L_0801f3e2:
	cmp r3, #0
	bge .L_0801f3e8
	adds r3, #3
.L_0801f3e8:
	asrs r3, r3, #2
	cmp r7, r3
	bgt .L_0801f3f6
	movs r0, #4
	bl UiWork_SetParamNibble
	b .L_0801f3fc
.L_0801f3f6:
	movs r0, #15
	bl UiWork_SetParamNibble
.L_0801f3fc:
	ldr r2, [sp, #32]
	ldr r3, .L_0801f5bc
	adds r6, r2, r3
	movs r3, #14
	strb r3, [r6]
	ldr r4, [sp, #32]
	ldr r0, .L_0801f5c0
	adds r3, r4, r0
	ldrb r3, [r3]
	movs r2, #0
	cmp r3, #0
	beq .L_0801f418
	movs r3, #5
	strb r3, [r6]
.L_0801f418:
	ldr r3, [sp, #20]
	str r2, [sp, #0]
	mov r1, r8
	mov r2, r9
	adds r3, #8
	adds r0, r7, #0
	bl UiText_DrawPrefixedNumberAtOffset
	movs r3, #15
	strb r3, [r6]
	mov r1, r8
	mov r2, r9
	adds r0, r5, #0
	ldr r3, [sp, #20]
	bl UiText_DrawStringAtOffset
	movs r0, #15
	bl UiWork_SetParamNibble
	movs r2, #52
	ldrsh r1, [r5, r2]
	cmp r1, #0
	beq .L_0801f46c
	movs r3, #56
	ldrsh r6, [r5, r3]
	lsls r0, r6, #2
	adds r0, r0, r6
	lsls r0, r0, #3
	bl __divsi3
	adds r3, r0, #0
	cmp r3, #0
	bne .L_0801f460
	cmp r6, #0
	beq .L_0801f460
	movs r3, #1
.L_0801f460:
	ldr r2, [sp, #24]
	mov r0, r8
	ldr r1, [sp, #8]
	adds r2, #2
	bl UiWindow_DrawStatusBarTiles
.L_0801f46c:
	movs r1, #1
	mov r3, r11
	ands r3, r1
	cmp r3, #0
	beq .L_0801f4ce
	ldr r4, [sp, #32]
	ldr r0, .L_0801f5bc
	movs r3, #14
	adds r2, r4, r0
	strb r3, [r2]
	subs r0, #2
	adds r3, r4, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0801f48e
	movs r3, #5
	strb r3, [r2]
.L_0801f48e:
	ldr r3, [sp, #12]
	movs r2, #58
	ldrsh r0, [r5, r2]
	adds r3, #16
	str r1, [sp, #0]
	mov r2, r9
	mov r1, r8
	bl UiText_DrawPrefixedNumberAtOffset
	movs r3, #54
	ldrsh r1, [r5, r3]
	cmp r1, #0
	beq .L_0801f4ce
	movs r4, #58
	ldrsh r6, [r5, r4]
	lsls r0, r6, #2
	adds r0, r0, r6
	lsls r0, r0, #3
	bl __divsi3
	adds r3, r0, #0
	cmp r3, #0
	bne .L_0801f4c2
	cmp r6, #0
	beq .L_0801f4c2
	movs r3, #1
.L_0801f4c2:
	ldr r2, [sp, #24]
	mov r0, r8
	ldr r1, [sp, #8]
	adds r2, #3
	bl UiWindow_DrawStatusBarTiles
.L_0801f4ce:
	ldr r0, [sp, #8]
	ldr r2, [sp, #4]
	movs r3, #1
	ldr r4, [sp, #28]
	adds r0, #6
	movs r1, #48
	adds r2, #2
	add r10, r3
	str r0, [sp, #8]
	add r9, r1
	str r2, [sp, #4]
	cmp r10, r4
	beq .L_0801f4ea
	b .L_0801f3c2
.L_0801f4ea:
	ldr r0, [sp, #32]
	ldr r1, .L_0801f5bc
	movs r3, #0
	adds r2, r0, r1
	mov r10, r3
	ldr r4, .L_0801f5c0
	movs r3, #15
	strb r3, [r2]
	adds r3, r0, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0801f59c
	movs r3, #2
	mov r0, r11
	ands r3, r0
	cmp r3, #0
	beq .L_0801f59c
	movs r3, #1
	ands r3, r0
	ldr r7, [sp, #24]
	cmp r3, #0
	beq .L_0801f518
	adds r7, #1
.L_0801f518:
	add r6, sp, #36
	adds r1, r6, #0
	movs r0, #0
	bl BattlePlacement_CountValidEntriesFar
	mov r2, r10
	str r2, [sp, #0]
	ldr r1, .L_0801f5c4
	mov r0, r8
	movs r2, #0
	adds r3, r7, #0
	bl UiWindow_SetTilemapEntry
	mov r3, r10
	str r3, [sp, #0]
	ldr r1, .L_0801f5c8
	mov r0, r8
	movs r2, #2
	adds r3, r7, #0
	bl UiWindow_SetTilemapEntry
	adds r5, r7, #1
	mov r4, r10
	ldr r1, .L_0801f5cc
	mov r0, r8
	movs r2, #0
	adds r3, r5, #0
	str r4, [sp, #0]
	bl UiWindow_SetTilemapEntry
	mov r0, r10
	str r0, [sp, #0]
	ldr r1, .L_0801f5d0
	mov r0, r8
	movs r2, #2
	adds r3, r5, #0
	bl UiWindow_SetTilemapEntry
	ldrb r1, [r6]
	movs r2, #1
	adds r1, #48
	adds r3, r7, #0
	mov r0, r8
	bl UiWindow_PutGlyph
	ldrb r1, [r6, #1]
	movs r2, #3
	adds r1, #48
	adds r3, r7, #0
	mov r0, r8
	bl UiWindow_PutGlyph
	ldrb r1, [r6, #2]
	movs r2, #1
	adds r1, #48
	adds r3, r5, #0
	mov r0, r8
	bl UiWindow_PutGlyph
	ldrb r1, [r6, #3]
	mov r0, r8
	adds r1, #48
	movs r2, #3
	adds r3, r5, #0
	bl UiWindow_PutGlyph
.L_0801f59c:
	ldr r1, [sp, #32]
	ldr r3, .L_0801f5b8
	adds r2, r1, r3
	movs r3, #0
	strb r3, [r2]
.L_0801f5a6:
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_0801f5b8:
	.4byte 0x00000ea6
.L_0801f5bc:
	.4byte 0x00000ea7
.L_0801f5c0:
	.4byte 0x00000ea5
.L_0801f5c4:
	.4byte 0x00005001
.L_0801f5c8:
	.4byte 0x00005002
.L_0801f5cc:
	.4byte 0x00005003
.L_0801f5d0:
	.4byte 0x00005004
