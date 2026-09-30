.syntax unified
	.thumb
	.global DjinnMenu_ShowChangePreview
	.thumb_func
DjinnMenu_ShowChangePreview:
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
	bl Owner_GetStateFar
	str r0, [sp, #56]
	mov r1, r10
	ldr r0, [r1]
	cmp r0, #0
	beq .L_08022b76
	movs r1, #1
	bl UiWork_Finalize
.L_08022b76:
	ldr r2, [sp, #60]
	cmp r2, #0
	bne .L_08022b92
	movs r3, #6
	str r3, [sp, #0]
	movs r0, #0
	movs r3, #11
	movs r1, #8
	movs r2, #21
	bl UiWindow_Create
	mov r3, r10
	str r0, [r3]
	b .L_08022ba6
.L_08022b92:
	movs r3, #6
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #5
	movs r2, #21
	movs r3, #14
	bl UiWindow_Create
	mov r4, r10
	str r0, [r4]
.L_08022ba6:
	mov r1, r10
	ldr r3, [r1]
	movs r0, #0
	cmp r3, #0
	bne .L_08022bb2
	b .L_08023134
.L_08022bb2:
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
	ldr r3, .L_08022e54
	ldr r1, [sp, #56]
	ldr r0, [sp, #52]
	bl _call_via_r3
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
	beq .L_08022c00
	adds r0, r7, #0
	adds r1, r5, #0
	adds r2, r6, #0
	bl Djinn_DeactivateFar
	b .L_08022c0a
.L_08022c00:
	adds r0, r7, #0
	adds r1, r5, #0
	adds r2, r6, #0
	bl Djinn_ActivateFar
.L_08022c0a:
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
	bl DjinnMenu_ListChangedDjinn
	movs r1, #5
	str r0, [sp, #20]
	subs r0, #1
	bl FixedPoint_Ratio
	ldr r3, [sp, #108]
	adds r0, #1
	str r0, [r3]
	ldr r4, [sp, #60]
	lsls r3, r4, #2
	adds r3, r3, r4
	ldr r1, [sp, #20]
	subs r3, #5
	cmp r3, r1
	blt .L_08022c44
	str r0, [sp, #60]
.L_08022c44:
	ldr r2, [sp, #60]
	cmp r2, #0
	beq .L_08022c4c
	b .L_08022d8c
.L_08022c4c:
	ldr r5, .L_08022e58
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
	movs r4, #56
	ldrsh r3, [r3, r4]
	ldr r0, [sp, #48]
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
	movs r2, #58
	ldrsh r1, [r1, r2]
	ldr r0, [sp, #48]
	str r1, [sp, #40]
	bl UiText_FormatNumberToHalfwords
	mov r2, r10
	ldr r1, [r2]
	adds r0, r5, #0
	movs r2, #5
	movs r3, #2
	bl UiText_RenderWideStringInWindow
	ldr r3, [sp, #52]
	ldrh r3, [r3, #60]
	ldr r0, [sp, #48]
	adds r1, r3, #0
	str r3, [sp, #36]
	bl UiText_FormatNumberToHalfwords
	ldr r5, [sp, #48]
	mov r4, r10
	adds r5, #16
	ldr r1, [r4]
	movs r2, #6
	movs r3, #3
	adds r0, r5, #0
	bl UiText_RenderWideStringInWindow
	ldr r1, [sp, #52]
	ldrh r1, [r1, #62]
	ldr r0, [sp, #48]
	str r1, [sp, #32]
	bl UiText_FormatNumberToHalfwords
	mov r2, r10
	ldr r1, [r2]
	movs r3, #4
	movs r2, #6
	adds r0, r5, #0
	bl UiText_RenderWideStringInWindow
	ldr r3, [sp, #52]
	adds r3, #64
	ldrh r3, [r3]
	ldr r0, [sp, #48]
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
	adds r3, #66
	ldrb r1, [r3]
	ldr r0, [sp, #48]
	bl UiText_FormatNumberToHalfwords
	mov r4, r10
	ldr r1, [r4]
	movs r2, #5
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
	bne .L_08022d72
	ldr r3, [sp, #64]
	cmp r3, #0
	beq .L_08022d78
.L_08022d72:
	movs r0, #2
	bl UiWork_SetParamNibble
.L_08022d78:
	mov r2, r10
	ldr r0, .L_08022e5c
	ldr r1, [r2]
	movs r3, #64
	movs r2, #24
	bl UiText_DrawCharacterAtOffset
	movs r0, #15
	bl UiWork_SetParamNibble
.L_08022d8c:
	ldr r3, [sp, #60]
	cmp r3, #0
	bgt .L_08022d94
	b .L_08022f54
.L_08022d94:
	ldr r1, [sp, #20]
	movs r4, #0
	str r4, [sp, #12]
	str r1, [sp, #8]
	cmp r1, #4
	ble .L_08022da4
	movs r2, #5
	str r2, [sp, #8]
.L_08022da4:
	ldr r4, [sp, #60]
	lsls r3, r4, #2
	adds r3, r3, r4
	subs r3, #5
	mov r8, r3
	ldr r2, [sp, #12]
	ldr r3, [sp, #8]
	movs r1, #0
	str r1, [sp, #16]
	cmp r2, r3
	blt .L_08022dbc
	b .L_08022ee0
.L_08022dbc:
	ldr r4, [sp, #20]
	cmp r8, r4
	blt .L_08022dc4
	b .L_08022ee0
.L_08022dc4:
	mov r1, r8
	ldr r2, [sp, #24]
	movs r4, #4
	lsls r3, r1, #1
	negs r4, r4
	adds r6, r3, r2
	str r4, [sp, #4]
	movs r3, #0
	mov r7, r10
	mov r9, r3
	mov r11, r3
.L_08022dda:
	ldrh r0, [r6]
	bl Ability_GetData
	adds r5, r0, #0
	ldrb r3, [r5, #2]
	cmp r3, #4
	beq .L_08022dfe
	mov r1, r10
	ldr r2, .L_08022e60
	ldr r0, [r1]
	adds r1, r3, #0
	movs r3, #0
	adds r1, r1, r2
	str r3, [sp, #0]
	movs r2, #15
	mov r3, r9
	bl UiWindow_SetTilemapEntry
.L_08022dfe:
	ldrb r3, [r5, #8]
	cmp r3, #255
	bne .L_08022e08
	movs r3, #11
	b .L_08022e0a
.L_08022e08:
	subs r3, #1
.L_08022e0a:
	movs r4, #0
	ldr r0, [r7]
	movs r1, #16
	mov r2, r9
	str r4, [sp, #0]
	bl UiWindow_DrawThreeTileColumn
	ldrh r3, [r6]
	ldr r1, .L_08022e64
	ldr r2, [sp, #4]
	ands r3, r1
	ldr r0, [r7]
	movs r1, #0
	bl Ui_CreateOutputFromResourceSlot
	ldrh r2, [r6]
	ldr r3, .L_08022e4c
	ands r3, r2
	cmp r3, #0
	beq .L_08022e3a
	movs r0, #4
	bl UiWork_SetParamNibble
	b .L_08022e6e
.L_08022e3a:
	ldr r3, .L_08022e50
	ands r3, r2
	cmp r3, #0
	beq .L_08022e68
	movs r0, #2
	bl UiWork_SetParamNibble
	b .L_08022e6e
	.2byte 0x0000
.L_08022e4c:
	.4byte 0x00008000
.L_08022e50:
	.4byte 0x00004000
.L_08022e54:
	.4byte IwramCopyWords
.L_08022e58:
	.4byte 0x000008ae
.L_08022e5c:
	.4byte 0x000008ad
.L_08022e60:
	.4byte 0x00005001
.L_08022e64:
	.4byte 0x00003fff
.L_08022e68:
	movs r0, #15
	bl UiWork_SetParamNibble
.L_08022e6e:
	ldrh r3, [r6]
	ldr r0, .L_08023148
	ands r0, r3
	ldr r3, .L_0802314c
	ldr r1, [r7]
	adds r0, r0, r3
	movs r2, #16
	mov r3, r11
	bl UiText_DrawCharacterAtOffset
	movs r2, #0
	ldr r0, [r7]
	mov r3, r9
	str r2, [sp, #0]
	ldr r1, .L_08023150
	movs r2, #11
	bl UiWindow_SetTilemapEntry
	movs r3, #0
	ldr r0, [r7]
	ldr r1, .L_08023154
	str r3, [sp, #0]
	movs r2, #12
	mov r3, r9
	bl UiWindow_SetTilemapEntry
	ldrh r0, [r6]
	bl Ability_GetData
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
	movs r1, #2
	add r11, r2
	ldr r2, [sp, #8]
	add r9, r1
	adds r3, #16
	adds r4, #1
	movs r1, #1
	str r3, [sp, #4]
	str r4, [sp, #16]
	adds r6, #2
	add r8, r1
	cmp r4, r2
	bge .L_08022ee0
	ldr r3, [sp, #20]
	cmp r8, r3
	bge .L_08022ee0
	b .L_08022dda
.L_08022ee0:
	ldr r3, [sp, #68]
	cmp r3, #0
	beq .L_08022efe
	movs r0, #4
	bl UiWork_SetParamNibble
	mov r4, r10
	ldr r1, [r4]
	ldr r0, .L_08023158
	movs r2, #32
	movs r3, #80
	bl UiText_DrawCharacterAtOffset
	movs r1, #1
	str r1, [sp, #12]
.L_08022efe:
	ldr r3, [sp, #64]
	cmp r3, #0
	beq .L_08022f22
	movs r0, #2
	bl UiWork_SetParamNibble
	ldr r4, [sp, #12]
	mov r2, r10
	lsls r3, r4, #3
	ldr r1, [r2]
	ldr r0, .L_0802315c
	adds r3, #80
	movs r2, #32
	bl UiText_DrawCharacterAtOffset
	ldr r1, [sp, #12]
	adds r1, #1
	str r1, [sp, #12]
.L_08022f22:
	ldr r2, [sp, #12]
	cmp r2, #0
	bne .L_08022f36
	mov r3, r10
	ldr r1, [r3]
	ldr r0, .L_08023160
	movs r2, #32
	movs r3, #80
	bl UiText_DrawCharacterAtOffset
.L_08022f36:
	movs r0, #15
	bl UiWork_SetParamNibble
	movs r0, #15
	bl UiWork_SetParamNibble
	movs r3, #10
	mov r4, r10
	ldr r0, [r4]
	movs r1, #0
	str r3, [sp, #0]
	movs r2, #10
	movs r3, #19
	bl UiWindow_DrawDividerLine
.L_08022f54:
	ldr r1, [sp, #60]
	cmp r1, #0
	beq .L_08022f5c
	b .L_08023110
.L_08022f5c:
	ldr r2, [sp, #52]
	ldr r5, .L_08023164
	adds r2, r2, r5
	ldrb r0, [r2]
	ldr r6, .L_08023168
	mov r3, r10
	ldr r1, [r3]
	mov r8, r2
	adds r0, r0, r6
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	ldr r4, [sp, #56]
	adds r5, r4, r5
	ldrb r0, [r5]
	mov r2, r10
	ldr r1, [r2]
	movs r3, #0
	movs r2, #80
	adds r0, r0, r6
	bl UiText_DrawCharacterAtOffset
	mov r3, r8
	ldrb r2, [r3]
	ldrb r3, [r5]
	cmp r2, r3
	beq .L_08022fa8
	ldr r2, [sp, #60]
	mov r4, r10
	ldr r0, [r4]
	ldr r1, .L_0802316c
	str r2, [sp, #0]
	movs r3, #0
	movs r2, #9
	bl UiWindow_SetTilemapEntry
	b .L_08022fba
.L_08022fa8:
	mov r3, r10
	ldr r4, [sp, #60]
	ldr r0, [r3]
	ldr r1, .L_08023170
	movs r2, #9
	movs r3, #0
	str r4, [sp, #0]
	bl UiWindow_SetTilemapEntry
.L_08022fba:
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
	beq .L_08022ff8
	movs r2, #0
	cmp r3, r4
	ble .L_08022fec
	movs r2, #1
.L_08022fec:
	add r1, sp, #76
	mov r9, r1
	movs r0, #80
	movs r1, #14
	bl DjinnMenu_DrawStatArrow
.L_08022ff8:
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
	beq .L_08023032
	movs r2, #0
	cmp r3, r4
	ble .L_08023026
	movs r2, #1
.L_08023026:
	add r1, sp, #76
	mov r9, r1
	movs r0, #80
	movs r1, #22
	bl DjinnMenu_DrawStatArrow
.L_08023032:
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
	beq .L_08023068
	movs r2, #0
	cmp r3, r1
	ble .L_0802305c
	movs r2, #1
.L_0802305c:
	add r3, sp, #76
	mov r9, r3
	movs r0, #80
	movs r1, #30
	bl DjinnMenu_DrawStatArrow
.L_08023068:
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
	beq .L_0802309e
	movs r2, #0
	cmp r3, r1
	ble .L_08023092
	movs r2, #1
.L_08023092:
	add r3, sp, #76
	mov r9, r3
	movs r0, #80
	movs r1, #38
	bl DjinnMenu_DrawStatArrow
.L_0802309e:
	ldr r5, [sp, #56]
	adds r5, #64
	ldrh r1, [r5]
	ldr r0, [sp, #48]
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
	beq .L_080230d4
	movs r2, #0
	cmp r3, r1
	ble .L_080230c8
	movs r2, #1
.L_080230c8:
	add r3, sp, #76
	mov r9, r3
	movs r0, #80
	movs r1, #46
	bl DjinnMenu_DrawStatArrow
.L_080230d4:
	ldr r5, [sp, #56]
	adds r5, #66
	ldrb r1, [r5]
	ldr r0, [sp, #48]
	bl UiText_FormatNumberToHalfwords
	ldr r0, [sp, #48]
	mov r4, r10
	ldr r1, [r4]
	movs r3, #6
	adds r0, #16
	movs r2, #12
	bl UiText_RenderWideStringInWindow
	ldr r3, [sp, #52]
	adds r3, #66
	ldrb r1, [r5]
	ldrb r3, [r3]
	cmp r1, r3
	beq .L_08023110
	movs r2, #0
	cmp r1, r3
	bls .L_08023104
	movs r2, #1
.L_08023104:
	add r1, sp, #76
	mov r9, r1
	movs r0, #80
	movs r1, #54
	bl DjinnMenu_DrawStatArrow
.L_08023110:
	movs r2, #166
	lsls r2, r2, #1
	ldr r3, .L_08023174
	ldr r0, [sp, #56]
	ldr r1, [sp, #52]
	bl _call_via_r3
	ldr r0, [sp, #24]
	bl Runtime_BumpFree
	ldr r0, [sp, #52]
	bl Runtime_BumpFree
	ldr r0, [sp, #48]
	bl Runtime_BumpFree
	mov r2, r10
	ldr r0, [r2]
.L_08023134:
	add sp, #76
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_08023148:
	.4byte 0x00003fff
.L_0802314c:
	.4byte 0x00000333
.L_08023150:
	.4byte 0x0000f01f
.L_08023154:
	.4byte 0x0000f01e
.L_08023158:
	.4byte 0x00000ba2
.L_0802315c:
	.4byte 0x00000ba3
.L_08023160:
	.4byte 0x00000ba8
.L_08023164:
	.4byte 0x00000129
.L_08023168:
	.4byte 0x00000741
.L_0802316c:
	.4byte 0x0000f728
.L_08023170:
	.4byte 0x0000f729
.L_08023174:
	.4byte IwramCopyWords
