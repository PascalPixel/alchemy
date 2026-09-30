.syntax unified
	.thumb
	.global Func_080464dc
	.thumb_func
Func_080464dc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #76
	add r4, sp, #72
	adds r7, r1, #0
	str r0, [r4]
	adds r0, r7, #0
	mov r10, r4
	mov r8, r2
	str r3, [sp, #60]
	bl Owner_GetState
	str r0, [sp, #56]
	mov r1, r10
	ldr r0, [r1]
	cmp r0, #0
	beq .L_0804650e
	movs r1, #1
	bl UiWork_Finalize
.L_0804650e:
	ldr r2, [sp, #60]
	cmp r2, #0
	bne .L_0804652a
	movs r3, #6
	str r3, [sp, #0]
	movs r0, #0
	movs r3, #11
	movs r1, #8
	movs r2, #20
	bl UiWindow_Create
	mov r3, r10
	str r0, [r3]
	b .L_0804653e
.L_0804652a:
	movs r3, #6
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #5
	movs r2, #21
	movs r3, #14
	bl UiWindow_Create
	mov r4, r10
	str r0, [r4]
.L_0804653e:
	mov r1, r10
	ldr r3, [r1]
	movs r0, #0
	cmp r3, #0
	bne .L_0804654a
	b .L_08046ae2
.L_0804654a:
	movs r0, #128
	bl Runtime_BumpAllocate
	movs r5, #166
	lsls r5, r5, #1
	str r0, [sp, #48]
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	str r0, [sp, #52]
	movs r0, #96
	bl Runtime_BumpAllocateAlternatePool
	adds r2, r5, #0
	str r0, [sp, #24]
	ldr r3, .L_080467f4
	ldr r1, [sp, #56]
	ldr r0, [sp, #52]
	mov lr, r3
	.2byte 0xf800
	mov r2, r8
	asrs r5, r2, #8
	movs r3, #15
	movs r6, #255
	ands r5, r3
	ands r6, r2
	adds r0, r7, #0
	adds r1, r5, #0
	adds r2, r6, #0
	bl Djinn_IsActiveFar
	cmp r0, #0
	beq .L_08046598
	adds r0, r7, #0
	adds r1, r5, #0
	adds r2, r6, #0
	bl Djinn_DeactivateFar
	b .L_080465a2
.L_08046598:
	adds r0, r7, #0
	adds r1, r5, #0
	adds r2, r6, #0
	bl Djinn_ActivateFar
.L_080465a2:
	adds r0, r7, #0
	bl Owner_RecalculateStatsFar
	ldr r0, [sp, #52]
	ldr r1, [sp, #56]
	add r2, sp, #64
	add r3, sp, #68
	str r2, [sp, #0]
	adds r1, #88
	ldr r2, [sp, #24]
	adds r0, #88
	bl Func_08046284
	movs r1, #5
	str r0, [sp, #20]
	subs r0, #1
	bl Math_Div
	ldr r3, [sp, #108]
	adds r0, #1
	str r0, [r3]
	ldr r4, [sp, #60]
	ldr r1, [sp, #20]
	lsls r3, r4, #2
	adds r3, r3, r4
	subs r3, #5
	cmp r3, r1
	blt .L_080465dc
	str r0, [sp, #60]
.L_080465dc:
	ldr r2, [sp, #60]
	cmp r2, #0
	beq .L_080465e4
	b .L_08046724
.L_080465e4:
	ldr r5, .L_080467f8
	mov r3, r10
	ldr r1, [r3]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	mov r4, r10
	ldr r1, [r4]
	adds r0, r5, #1
	movs r2, #0
	movs r3, #16
	bl UiText_DrawCharacterAtOffset
	mov r2, r10
	adds r0, r5, #2
	ldr r1, [r2]
	movs r3, #24
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	mov r3, r10
	adds r0, r5, #3
	ldr r1, [r3]
	movs r2, #0
	movs r3, #32
	bl UiText_DrawCharacterAtOffset
	mov r4, r10
	ldr r1, [r4]
	adds r0, r5, #4
	movs r2, #0
	movs r3, #40
	bl UiText_DrawCharacterAtOffset
	adds r5, #5
	mov r2, r10
	ldr r1, [r2]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #48
	bl UiText_DrawCharacterAtOffset
	ldr r3, [sp, #52]
	ldr r0, [sp, #48]
	movs r4, #56
	ldrsh r3, [r3, r4]
	adds r1, r3, #0
	str r3, [sp, #44]
	bl UiText_FormatNumberToHalfwords
	ldr r5, [sp, #48]
	mov r4, r10
	adds r5, #14
	ldr r1, [r4]
	movs r2, #5
	movs r3, #1
	adds r0, r5, #0
	bl UiText_RenderWideStringInWindow
	ldr r1, [sp, #52]
	ldr r0, [sp, #48]
	movs r2, #58
	ldrsh r1, [r1, r2]
	str r1, [sp, #40]
	bl UiText_FormatNumberToHalfwords
	mov r2, r10
	ldr r1, [r2]
	adds r0, r5, #0
	movs r2, #5
	movs r3, #2
	bl UiText_RenderWideStringInWindow
	ldr r3, [sp, #52]
	ldr r0, [sp, #48]
	ldrh r3, [r3, #60]
	adds r1, r3, #0
	str r3, [sp, #36]
	bl UiText_FormatNumberToHalfwords
	mov r4, r10
	ldr r1, [r4]
	movs r2, #5
	movs r3, #3
	adds r0, r5, #0
	bl UiText_RenderWideStringInWindow
	ldr r1, [sp, #52]
	ldr r0, [sp, #48]
	ldrh r1, [r1, #62]
	str r1, [sp, #32]
	bl UiText_FormatNumberToHalfwords
	ldr r5, [sp, #48]
	mov r2, r10
	adds r5, #16
	ldr r1, [r2]
	movs r3, #4
	movs r2, #6
	adds r0, r5, #0
	bl UiText_RenderWideStringInWindow
	ldr r3, [sp, #52]
	ldr r0, [sp, #48]
	adds r3, #64
	ldrh r3, [r3]
	adds r1, r3, #0
	str r3, [sp, #28]
	bl UiText_FormatNumberToHalfwords
	mov r3, r10
	ldr r1, [r3]
	movs r2, #6
	movs r3, #5
	adds r0, r5, #0
	bl UiText_RenderWideStringInWindow
	ldr r3, [sp, #52]
	ldr r0, [sp, #48]
	adds r3, #66
	ldrb r1, [r3]
	bl UiText_FormatNumberToHalfwords
	mov r4, r10
	ldr r1, [r4]
	movs r2, #6
	movs r3, #6
	adds r0, r5, #0
	bl UiText_RenderWideStringInWindow
	mov r1, r10
	movs r3, #8
	ldr r0, [r1]
	movs r2, #8
	str r3, [sp, #0]
	movs r1, #0
	movs r3, #19
	bl UiWindow_DrawDividerLine
	ldr r3, [sp, #68]
	cmp r3, #0
	bne .L_0804670a
	ldr r3, [sp, #64]
	cmp r3, #0
	beq .L_08046710
.L_0804670a:
	movs r0, #2
	bl Func_08041f70
.L_08046710:
	mov r2, r10
	ldr r0, .L_080467fc
	ldr r1, [r2]
	movs r3, #64
	movs r2, #24
	bl UiText_DrawCharacterAtOffset
	movs r0, #15
	bl Func_08041f70
.L_08046724:
	ldr r3, [sp, #60]
	cmp r3, #0
	bgt .L_0804672c
	b .L_080468f8
.L_0804672c:
	ldr r1, [sp, #20]
	movs r4, #0
	str r4, [sp, #12]
	str r1, [sp, #8]
	cmp r1, #4
	ble .L_0804673c
	movs r2, #5
	str r2, [sp, #8]
.L_0804673c:
	ldr r4, [sp, #60]
	ldr r2, [sp, #12]
	lsls r3, r4, #2
	adds r3, r3, r4
	subs r3, #5
	mov r8, r3
	ldr r3, [sp, #8]
	movs r1, #0
	str r1, [sp, #16]
	cmp r2, r3
	blt .L_08046754
	b .L_08046884
.L_08046754:
	ldr r4, [sp, #20]
	cmp r8, r4
	blt .L_0804675c
	b .L_08046884
.L_0804675c:
	ldr r2, [sp, #24]
	movs r4, #4
	mov r1, r8
	negs r4, r4
	lsls r3, r1, #1
	str r4, [sp, #4]
	adds r6, r3, r2
	movs r3, #0
	mov r7, r10
	mov r9, r3
	mov r11, r3
.L_08046772:
	ldrh r0, [r6]
	bl BattleAction_Get
	adds r5, r0, #0
	ldrb r3, [r5, #2]
	cmp r3, #4
	beq .L_0804679a
	movs r2, #160
	mov r1, r10
	lsls r2, r2, #7
	ldr r0, [r1]
	adds r2, #1
	adds r1, r3, #0
	movs r3, #0
	adds r1, r1, r2
	str r3, [sp, #0]
	movs r2, #15
	mov r3, r9
	bl UiWindow_SetTilemapEntry
.L_0804679a:
	ldrb r3, [r5, #8]
	cmp r3, #255
	bne .L_080467a4
	movs r3, #11
	b .L_080467a6
.L_080467a4:
	subs r3, #1
.L_080467a6:
	movs r4, #0
	ldr r0, [r7]
	movs r1, #16
	mov r2, r9
	str r4, [sp, #0]
	bl UiWindow_DrawThreeTileColumn
	ldrh r3, [r6]
	movs r1, #252
	lsls r1, r1, #6
	adds r1, #255
	ands r3, r1
	ldr r2, [sp, #4]
	ldr r0, [r7]
	movs r1, #0
	bl Ui_CreateOutputFromResourceSlot
	ldrh r2, [r6]
	ldr r3, .L_080467ec
	ands r3, r2
	cmp r3, #0
	beq .L_080467da
	movs r0, #4
	bl Func_08041f70
	b .L_08046806
.L_080467da:
	ldr r3, .L_080467f0
	ands r3, r2
	cmp r3, #0
	beq .L_08046800
	movs r0, #2
	bl Func_08041f70
	b .L_08046806
	.2byte 0x0000
.L_080467ec:
	.4byte 0x00008000
.L_080467f0:
	.4byte 0x00004000
.L_080467f4:
	.4byte IwramCopyWords
.L_080467f8:
	.4byte 0x00000d0e
.L_080467fc:
	.4byte 0x00000d0d
.L_08046800:
	movs r0, #15
	bl Func_08041f70
.L_08046806:
	ldrh r3, [r6]
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	ands r0, r3
	ldr r3, .L_08046af0
	ldr r1, [r7]
	adds r0, r0, r3
	movs r2, #16
	mov r3, r11
	bl UiText_DrawCharacterAtOffset
	movs r1, #240
	movs r2, #0
	lsls r1, r1, #8
	ldr r0, [r7]
	mov r3, r9
	str r2, [sp, #0]
	adds r1, #31
	movs r2, #11
	bl UiWindow_SetTilemapEntry
	movs r1, #240
	movs r3, #0
	lsls r1, r1, #8
	ldr r0, [r7]
	adds r1, #30
	str r3, [sp, #0]
	movs r2, #12
	mov r3, r9
	bl UiWindow_SetTilemapEntry
	ldrh r0, [r6]
	bl BattleAction_Get
	mov r4, r11
	ldr r2, [r7]
	ldrb r0, [r0, #9]
	movs r1, #2
	movs r3, #104
	str r4, [sp, #0]
	bl UiText_DrawNumberInWindow
	movs r2, #16
	ldr r3, [sp, #4]
	ldr r4, [sp, #16]
	add r11, r2
	ldr r2, [sp, #8]
	movs r1, #2
	add r9, r1
	adds r3, #16
	adds r4, #1
	movs r1, #1
	str r3, [sp, #4]
	str r4, [sp, #16]
	adds r6, #2
	add r8, r1
	cmp r4, r2
	bge .L_08046884
	ldr r3, [sp, #20]
	cmp r8, r3
	bge .L_08046884
	b .L_08046772
.L_08046884:
	ldr r3, [sp, #68]
	cmp r3, #0
	beq .L_080468a2
	movs r0, #4
	bl Func_08041f70
	mov r4, r10
	ldr r1, [r4]
	ldr r0, .L_08046af4
	movs r2, #32
	movs r3, #80
	bl UiText_DrawCharacterAtOffset
	movs r1, #1
	str r1, [sp, #12]
.L_080468a2:
	ldr r3, [sp, #64]
	cmp r3, #0
	beq .L_080468c6
	movs r0, #2
	bl Func_08041f70
	ldr r4, [sp, #12]
	mov r2, r10
	lsls r3, r4, #3
	ldr r1, [r2]
	ldr r0, .L_08046af8
	adds r3, #80
	movs r2, #32
	bl UiText_DrawCharacterAtOffset
	ldr r1, [sp, #12]
	adds r1, #1
	str r1, [sp, #12]
.L_080468c6:
	ldr r2, [sp, #12]
	cmp r2, #0
	bne .L_080468da
	mov r3, r10
	ldr r1, [r3]
	ldr r0, .L_08046afc
	movs r2, #32
	movs r3, #80
	bl UiText_DrawCharacterAtOffset
.L_080468da:
	movs r0, #15
	bl Func_08041f70
	movs r0, #15
	bl Func_08041f70
	movs r3, #10
	mov r4, r10
	ldr r0, [r4]
	movs r1, #0
	str r3, [sp, #0]
	movs r2, #10
	movs r3, #19
	bl UiWindow_DrawDividerLine
.L_080468f8:
	ldr r1, [sp, #60]
	cmp r1, #0
	beq .L_08046900
	b .L_08046abe
.L_08046900:
	ldr r2, [sp, #52]
	movs r5, #42
	adds r5, #255
	adds r2, r2, r5
	ldrb r0, [r2]
	ldr r6, .L_08046b00
	mov r3, r10
	ldr r1, [r3]
	mov r8, r2
	adds r0, r0, r6
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	ldr r4, [sp, #56]
	mov r2, r10
	adds r5, r4, r5
	ldrb r0, [r5]
	ldr r1, [r2]
	movs r3, #0
	movs r2, #80
	adds r0, r0, r6
	bl UiText_DrawCharacterAtOffset
	mov r3, r8
	ldrb r2, [r3]
	ldrb r3, [r5]
	cmp r2, r3
	beq .L_08046952
	ldr r2, [sp, #60]
	movs r1, #247
	mov r4, r10
	lsls r1, r1, #8
	ldr r0, [r4]
	adds r1, #40
	str r2, [sp, #0]
	movs r3, #0
	movs r2, #9
	bl UiWindow_SetTilemapEntry
	b .L_08046968
.L_08046952:
	movs r1, #247
	ldr r4, [sp, #60]
	mov r3, r10
	lsls r1, r1, #8
	ldr r0, [r3]
	adds r1, #41
	movs r2, #9
	movs r3, #0
	str r4, [sp, #0]
	bl UiWindow_SetTilemapEntry
.L_08046968:
	ldr r3, [sp, #56]
	ldr r0, [sp, #48]
	movs r2, #56
	ldrsh r1, [r3, r2]
	bl UiText_FormatNumberToHalfwords
	ldr r6, [sp, #48]
	mov r4, r10
	adds r6, #14
	ldr r1, [r4]
	movs r2, #11
	movs r3, #1
	adds r0, r6, #0
	bl UiText_RenderWideStringInWindow
	ldr r2, [sp, #56]
	ldr r4, [sp, #44]
	movs r1, #56
	ldrsh r3, [r2, r1]
	cmp r3, r4
	beq .L_080469a6
	movs r2, #0
	cmp r3, r4
	ble .L_0804699a
	movs r2, #1
.L_0804699a:
	add r1, sp, #76
	mov r9, r1
	movs r0, #80
	movs r1, #14
	bl Func_08046414
.L_080469a6:
	ldr r3, [sp, #56]
	ldr r0, [sp, #48]
	movs r2, #58
	ldrsh r1, [r3, r2]
	bl UiText_FormatNumberToHalfwords
	mov r4, r10
	ldr r1, [r4]
	movs r2, #11
	movs r3, #2
	adds r0, r6, #0
	bl UiText_RenderWideStringInWindow
	ldr r2, [sp, #56]
	ldr r4, [sp, #40]
	movs r1, #58
	ldrsh r3, [r2, r1]
	cmp r3, r4
	beq .L_080469e0
	movs r2, #0
	cmp r3, r4
	ble .L_080469d4
	movs r2, #1
.L_080469d4:
	add r1, sp, #76
	mov r9, r1
	movs r0, #80
	movs r1, #22
	bl Func_08046414
.L_080469e0:
	ldr r2, [sp, #56]
	ldr r0, [sp, #48]
	ldrh r1, [r2, #60]
	bl UiText_FormatNumberToHalfwords
	mov r3, r10
	ldr r1, [r3]
	adds r0, r6, #0
	movs r3, #3
	movs r2, #11
	bl UiText_RenderWideStringInWindow
	ldr r4, [sp, #56]
	ldr r1, [sp, #36]
	ldrh r3, [r4, #60]
	cmp r3, r1
	beq .L_08046a16
	movs r2, #0
	cmp r3, r1
	ble .L_08046a0a
	movs r2, #1
.L_08046a0a:
	add r3, sp, #76
	mov r9, r3
	movs r0, #80
	movs r1, #30
	bl Func_08046414
.L_08046a16:
	ldr r4, [sp, #56]
	ldr r0, [sp, #48]
	ldrh r1, [r4, #62]
	bl UiText_FormatNumberToHalfwords
	mov r2, r10
	ldr r1, [r2]
	movs r3, #4
	adds r0, r6, #0
	movs r2, #11
	bl UiText_RenderWideStringInWindow
	ldr r4, [sp, #56]
	ldr r1, [sp, #32]
	ldrh r3, [r4, #62]
	cmp r3, r1
	beq .L_08046a4c
	movs r2, #0
	cmp r3, r1
	ble .L_08046a40
	movs r2, #1
.L_08046a40:
	add r3, sp, #76
	mov r9, r3
	movs r0, #80
	movs r1, #38
	bl Func_08046414
.L_08046a4c:
	ldr r5, [sp, #56]
	ldr r0, [sp, #48]
	adds r5, #64
	ldrh r1, [r5]
	bl UiText_FormatNumberToHalfwords
	mov r4, r10
	ldr r1, [r4]
	movs r3, #5
	adds r0, r6, #0
	movs r2, #11
	bl UiText_RenderWideStringInWindow
	ldrh r3, [r5]
	ldr r1, [sp, #28]
	cmp r3, r1
	beq .L_08046a82
	movs r2, #0
	cmp r3, r1
	ble .L_08046a76
	movs r2, #1
.L_08046a76:
	add r3, sp, #76
	mov r9, r3
	movs r0, #80
	movs r1, #46
	bl Func_08046414
.L_08046a82:
	ldr r5, [sp, #56]
	ldr r0, [sp, #48]
	adds r5, #66
	ldrb r1, [r5]
	bl UiText_FormatNumberToHalfwords
	ldr r0, [sp, #48]
	mov r4, r10
	ldr r1, [r4]
	movs r3, #6
	adds r0, #16
	movs r2, #12
	bl UiText_RenderWideStringInWindow
	ldr r3, [sp, #52]
	ldrb r1, [r5]
	adds r3, #66
	ldrb r3, [r3]
	cmp r1, r3
	beq .L_08046abe
	movs r2, #0
	cmp r1, r3
	bls .L_08046ab2
	movs r2, #1
.L_08046ab2:
	add r1, sp, #76
	mov r9, r1
	movs r0, #80
	movs r1, #54
	bl Func_08046414
.L_08046abe:
	movs r2, #166
	lsls r2, r2, #1
	ldr r3, .L_08046b04
	ldr r0, [sp, #56]
	ldr r1, [sp, #52]
	mov lr, r3
	.2byte 0xf800
	ldr r0, [sp, #24]
	bl Sys_Free
	ldr r0, [sp, #52]
	bl Sys_Free
	ldr r0, [sp, #48]
	bl Sys_Free
	mov r2, r10
	ldr r0, [r2]
.L_08046ae2:
	add sp, #76
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08046af0:
	.4byte 0x000005a7
.L_08046af4:
	.4byte 0x000010d3
.L_08046af8:
	.4byte 0x000010d4
.L_08046afc:
	.4byte 0x000010d9
.L_08046b00:
	.4byte 0x00000b63
.L_08046b04:
	.4byte IwramCopyWords
