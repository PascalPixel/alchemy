.syntax unified
	.thumb
	.global Func_080aafb8
Func_080aafb8:
	.global DjinnMenu_DrawElementList
	.thumb_func
DjinnMenu_DrawElementList:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #52
	str r0, [sp, #48]
	ldr r3, .L_080ab0a4
	ldr r0, [r3]
	subs r3, #160
	ldr r3, [r3]
	ldr r1, .L_080ab0a8
	str r3, [sp, #32]
	adds r2, r3, r1
	movs r3, #1
	strb r3, [r2]
	movs r2, #0
	ldr r3, .L_080ab0ac
	mov r9, r0
	str r2, [sp, #44]
	add r3, r9
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080ab01e
	ldr r7, [sp, #48]
	movs r6, #130
	lsls r6, r6, #2
	ldr r5, [sp, #48]
	adds r7, #160
	add r6, r9
.L_080aaff8:
	movs r2, #1
	ldrh r1, [r6]
	adds r0, r5, #0
	negs r2, r2
	bl Djinn_ListOwnerEntries
	strb r0, [r7]
	ldr r3, [sp, #44]
	adds r3, #1
	str r3, [sp, #44]
	ldr r3, .L_080ab0ac
	add r3, r9
	ldrb r3, [r3]
	ldr r0, [sp, #44]
	adds r6, #2
	adds r7, #1
	adds r5, #20
	cmp r0, r3
	blt .L_080aaff8
.L_080ab01e:
	mov r1, r9
	ldr r0, [r1, #48]
	bl RenderOutput_RedrawSavedRectFar
	mov r2, r9
	ldr r0, .L_080ab0b0
	ldr r1, [r2, #48]
	movs r3, #80
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #0
	str r3, [sp, #40]
	ldr r3, .L_080ab0ac
	add r3, r9
	ldrb r3, [r3]
	movs r0, #0
	cmp r0, r3
	blt .L_080ab046
	b .L_080ab1a2
.L_080ab046:
	ldr r3, [sp, #48]
	movs r2, #0
	movs r1, #160
	str r1, [sp, #16]
	str r2, [sp, #12]
	str r2, [sp, #8]
	str r3, [sp, #4]
.L_080ab054:
	movs r0, #0
	str r0, [sp, #28]
	str r0, [sp, #36]
.L_080ab05a:
	movs r1, #0
	str r1, [sp, #44]
	ldr r2, [sp, #16]
	ldr r0, [sp, #48]
	ldrsb r3, [r2, r0]
	cmp r1, r3
	blt .L_080ab06a
	b .L_080ab16a
.L_080ab06a:
	ldr r3, [sp, #12]
	ldr r0, [sp, #28]
	ldr r2, [sp, #8]
	str r3, [sp, #20]
	lsls r3, r0, #3
	movs r1, #224
	adds r3, #16
	str r2, [sp, #24]
	ldr r5, [sp, #4]
	mov r11, r1
	mov r10, r3
.L_080ab080:
	ldrh r4, [r5]
	mov r3, r11
	ands r3, r4
	ldr r1, [sp, #36]
	lsrs r3, r3, #5
	cmp r1, r3
	bne .L_080ab156
	ldr r3, .L_080ab0a0
	ands r3, r4
	cmp r3, #0
	bne .L_080ab0b4
	movs r0, #2
	bl UiWork_SetParamNibbleFar
	ldrh r4, [r5]
	b .L_080ab0b4
.L_080ab0a0:
	.4byte 0x00008000
.L_080ab0a4:
	.4byte gMenuWork
.L_080ab0a8:
	.4byte 0x00000ea6
.L_080ab0ac:
	.4byte 0x00000219
.L_080ab0b0:
	.4byte 0x00000bad
.L_080ab0b4:
	movs r7, #240
	lsls r7, r7, #4
	movs r2, #0
	adds r0, r7, #0
	mov r1, r11
	movs r6, #31
	mov r8, r2
	ands r0, r4
	ands r1, r4
	adds r2, r6, #0
	lsrs r0, r0, #8
	lsrs r1, r1, #5
	ands r2, r4
	bl Trade_CanOfferDjinnFar
	cmp r0, #0
	bne .L_080ab0f0
	ldrh r3, [r5]
	adds r0, r7, #0
	mov r1, r11
	ands r0, r3
	ands r1, r3
	adds r2, r6, #0
	lsrs r0, r0, #8
	lsrs r1, r1, #5
	ands r2, r3
	bl Djinn_IsActiveFar
	cmp r0, #0
	beq .L_080ab0f4
.L_080ab0f0:
	movs r3, #1
	mov r8, r3
.L_080ab0f4:
	mov r0, r8
	cmp r0, #0
	bne .L_080ab100
	movs r0, #4
	bl UiWork_SetParamNibbleFar
.L_080ab100:
	ldrh r3, [r5]
	mov r1, r9
	ldr r0, [r1, #48]
	mov r1, r11
	ands r1, r3
	ldr r2, .L_080ab1dc
	lsrs r1, r1, #5
	adds r1, r1, r2
	movs r2, #0
	ldr r3, [sp, #28]
	str r2, [sp, #0]
	ldr r2, [sp, #24]
	adds r3, #2
	adds r2, #1
	bl UiWindow_SetTilemapEntryFar
	ldrh r2, [r5]
	mov r3, r11
	ands r3, r2
	lsrs r3, r3, #5
	lsls r0, r3, #2
	adds r0, r0, r3
	movs r3, #31
	ands r3, r2
	lsls r0, r0, #2
	adds r0, r0, r3
	ldr r3, .L_080ab1e0
	ldr r2, [sp, #20]
	adds r0, r0, r3
	mov r3, r9
	ldr r1, [r3, #48]
	adds r2, #16
	mov r3, r10
	bl UiText_DrawCharacterAtOffsetFar
	ldr r1, [sp, #28]
	movs r0, #8
	add r10, r0
	adds r1, #1
	movs r0, #15
	str r1, [sp, #28]
	bl UiWork_SetParamNibbleFar
.L_080ab156:
	ldr r2, [sp, #44]
	adds r2, #1
	str r2, [sp, #44]
	ldr r0, [sp, #16]
	ldr r1, [sp, #48]
	ldrsb r3, [r0, r1]
	adds r5, #2
	cmp r2, r3
	bge .L_080ab16a
	b .L_080ab080
.L_080ab16a:
	ldr r2, [sp, #36]
	adds r2, #1
	str r2, [sp, #36]
	cmp r2, #3
	bgt .L_080ab176
	b .L_080ab05a
.L_080ab176:
	ldr r3, [sp, #16]
	adds r3, #1
	str r3, [sp, #16]
	ldr r3, [sp, #40]
	ldr r0, [sp, #12]
	ldr r1, [sp, #8]
	ldr r2, [sp, #4]
	adds r3, #1
	str r3, [sp, #40]
	adds r0, #56
	adds r1, #7
	adds r2, #20
	ldr r3, .L_080ab1e4
	str r0, [sp, #12]
	str r1, [sp, #8]
	str r2, [sp, #4]
	add r3, r9
	ldrb r3, [r3]
	ldr r0, [sp, #40]
	cmp r0, r3
	bge .L_080ab1a2
	b .L_080ab054
.L_080ab1a2:
	mov r1, r9
	movs r3, #10
	ldr r0, [r1, #48]
	movs r2, #10
	str r3, [sp, #0]
	movs r1, #0
	movs r3, #28
	bl UiWindow_DrawDividerLineFar
	ldr r3, .L_080ab1e8
	ldr r2, .L_080ab1ec
	ldr r3, [r3]
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3]
	ldr r0, [sp, #32]
	ldr r2, .L_080ab1f0
	movs r1, #0
	adds r3, r0, r2
	strb r1, [r3]
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080ab1dc:
	.4byte 0x00005001
.L_080ab1e0:
	.4byte 0x0000045f
.L_080ab1e4:
	.4byte 0x00000219
.L_080ab1e8:
	.4byte gWindowWork
.L_080ab1ec:
	.4byte 0x00000ea3
.L_080ab1f0:
	.4byte 0x00000ea6
