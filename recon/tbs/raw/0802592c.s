.syntax unified
	.thumb
	.global BattleMenu_RunActionSelection
	.thumb_func
BattleMenu_RunActionSelection:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #300
	str r1, [sp, #72]
	str r2, [sp, #68]
	ldr r5, .L_080259f0
	adds r6, r0, #0
	ldr r0, [r5]
	movs r1, #1
	str r0, [sp, #64]
	negs r1, r1
	movs r0, #128
	str r1, [sp, #56]
	mov r9, r1
	bl Resource_LoadIntoFreeSlot
	str r0, [sp, #52]
	adds r0, r6, #0
	bl Owner_GetStateFar
	movs r3, #42
	str r0, [sp, #48]
	str r3, [sp, #0]
	movs r1, #5
	movs r2, #30
	movs r3, #4
	movs r0, #0
	bl UiWindow_Create
	movs r2, #5
	str r0, [sp, #44]
	str r2, [sp, #40]
	adds r5, #168
	ldr r3, [r5]
	ldr r0, [r3, #52]
	str r0, [sp, #60]
	ldr r1, [r3, #48]
	ldr r3, [r3, #56]
	mov r10, r1
	str r3, [sp, #36]
	movs r3, #6
	str r3, [sp, #0]
	movs r2, #21
	movs r3, #11
	movs r0, #9
	movs r1, #9
	bl UiWindow_Create
	mov r2, sp
	adds r2, #80
	ldr r3, .L_080259f4
	movs r6, #128
	str r2, [sp, #12]
	mov r11, r0
	movs r7, #0
	mov r12, r3
	adds r4, r2, #0
	lsls r6, r6, #23
	movs r5, #0
.L_080259ac:
	lsls r0, r7, #1
	str r6, [r4, #4]
	str r5, [r4, #8]
	mov r1, r11
	ldrh r2, [r1, #12]
	ldr r3, .L_080259ec
	lsls r2, r2, #3
	ldrh r1, [r4, #6]
	adds r2, #8
	ands r2, r3
	mov r3, r12
	ands r3, r1
	orrs r3, r2
	mov r2, r11
	strh r3, [r4, #6]
	ldrh r3, [r2, #14]
	adds r0, r0, r3
	lsls r0, r0, #3
	adds r0, #4
	adds r7, #1
	strb r0, [r4, #4]
	adds r4, #12
	cmp r7, #4
	ble .L_080259ac
	mov r3, sp
	adds r3, #140
	ldr r0, .L_080259f8
	str r3, [sp, #8]
	ldr r6, [sp, #12]
	str r3, [sp, #4]
	movs r5, #8
	b .L_080259fc
.L_080259ec:
	.4byte 0x000001ff
.L_080259f0:
	.4byte gWindowWork
.L_080259f4:
	.4byte 0xfffffe00
.L_080259f8:
	.4byte 0xfffffc00
.L_080259fc:
	mov r8, r0
	movs r7, #4
.L_08025a00:
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	ldr r2, [sp, #4]
	stmia r2!, {r0}
	adds r1, r2, #0
	str r1, [sp, #4]
	movs r1, #1
	negs r1, r1
	bl Resource_GetBuffer
	ldr r3, .L_08025a4c
	ands r0, r3
	ldrh r3, [r5, r6]
	mov r1, r8
	ands r3, r1
	orrs r3, r0
	subs r7, #1
	strh r3, [r5, r6]
	adds r5, #12
	cmp r7, #0
	bge .L_08025a00
	ldr r5, .L_08025a50
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #2
	bl Vram_CopyTile
	adds r0, r5, #0
	ldr r1, .L_08025a54
	bl Vram_CopyTile
	adds r5, #1
	movs r1, #132
	lsls r1, r1, #2
	adds r0, r5, #0
	b .L_08025a58
	.2byte 0x0000
.L_08025a4c:
	.4byte 0x000003ff
.L_08025a50:
	.4byte 0x0000f018
.L_08025a54:
	.4byte 0x00000201
.L_08025a58:
	bl Vram_CopyTile
	ldr r1, .L_08025ad8
	adds r0, r5, #0
	bl Vram_CopyTile
	movs r2, #144
	lsls r2, r2, #1
	mov r3, r10
	add r2, sp
	lsls r3, r3, #1
	str r2, [sp, #24]
	str r3, [sp, #20]
.L_08025a72:
	ldr r0, [sp, #60]
	cmp r0, r9
	bne .L_08025a80
	ldr r1, [sp, #56]
	cmp r10, r1
	bne .L_08025a80
	b .L_08025cd6
.L_08025a80:
	ldr r3, [sp, #64]
	ldr r0, .L_08025adc
	adds r2, r3, r0
	movs r3, #1
	strb r3, [r2]
	ldr r2, [sp, #56]
	mov r1, r11
	ldrh r0, [r1, #12]
	ldrh r1, [r1, #14]
	lsls r3, r2, #1
	adds r1, r1, r3
	mov r3, r11
	ldrh r2, [r3, #8]
	movs r3, #15
	str r3, [sp, #0]
	adds r0, #1
	adds r1, #1
	subs r2, #2
	movs r3, #1
	bl Ui_SetRectHighlight
	bl Ui_FillVramBlockPattern
	ldr r0, [sp, #68]
	cmp r0, #0
	beq .L_08025ae4
	ldr r3, [sp, #60]
	ldr r1, [sp, #72]
	add r3, r10
	lsls r3, r3, #1
	ldrh r3, [r3, r1]
	ldr r0, .L_08025ad4
	ands r0, r3
	ldr r3, .L_08025ae0
	add r5, sp, #160
	adds r0, r0, r3
	adds r1, r5, #0
	movs r2, #52
	bl UiText_CopyMessageString
	b .L_08025af0
	.2byte 0x0000
.L_08025ad4:
	.4byte 0x00003fff
.L_08025ad8:
	.4byte 0x00000211
.L_08025adc:
	.4byte 0x00000ea6
.L_08025ae0:
	.4byte 0x0000053a
.L_08025ae4:
	add r5, sp, #160
	ldr r0, .L_08025b9c
	adds r1, r5, #0
	movs r2, #52
	bl UiText_CopyMessageString
.L_08025af0:
	movs r2, #0
	movs r3, #4
	adds r0, r5, #0
	ldr r1, [sp, #44]
	bl UiText_RenderWideStringAtOffset
	ldr r3, [sp, #60]
	mov r2, r10
	str r2, [sp, #56]
	cmp r3, r9
	bne .L_08025b08
	b .L_08025c5c
.L_08025b08:
	mov r0, r11
	bl RenderOutput_RedrawSavedRect
	ldr r0, [sp, #60]
	ldr r1, [sp, #72]
	lsls r3, r0, #1
	ldrh r5, [r3, r1]
	movs r7, #0
	cmp r5, #0
	bne .L_08025b1e
	b .L_08025c56
.L_08025b1e:
	mov r2, sp
	adds r2, #76
	str r2, [sp, #16]
.L_08025b24:
	adds r0, r5, #0
	bl Ability_GetData
	mov r8, r0
	movs r0, #0
	str r0, [sp, #0]
	lsls r3, r7, #1
	mov r0, r11
	ldr r1, .L_08025ba0
	movs r2, #11
	mov r9, r3
	bl UiWindow_SetTilemapEntry
	movs r1, #0
	str r1, [sp, #0]
	mov r0, r11
	ldr r1, .L_08025ba4
	movs r2, #12
	mov r3, r9
	bl UiWindow_SetTilemapEntry
	ldr r3, [sp, #8]
	ldr r0, .L_08025ba8
	lsls r2, r7, #2
	adds r2, r3, r2
	movs r3, #1
	ands r0, r5
	str r3, [sp, #0]
	movs r1, #0
	ldr r3, [sp, #16]
	bl Ability_LoadGlyph
	mov r0, r9
	adds r1, r0, r7
	ldr r2, [sp, #12]
	ldr r3, .L_08025b94
	lsls r1, r1, #2
	ldr r0, [sp, #76]
	adds r1, #8
	ands r0, r3
	ldrh r3, [r2, r1]
	ldr r2, .L_08025b98
	ands r3, r2
	orrs r3, r0
	ldr r0, [sp, #12]
	strh r3, [r0, r1]
	mov r1, r8
	ldrb r2, [r1, #1]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	bne .L_08025bac
	movs r0, #4
	bl UiWork_SetParamNibble
	b .L_08025bd4
.L_08025b94:
	.4byte 0x000003ff
.L_08025b98:
	.4byte 0xfffffc00
.L_08025b9c:
	.4byte 0x000008e7
.L_08025ba0:
	.4byte 0x0000f01f
.L_08025ba4:
	.4byte 0x0000f01e
.L_08025ba8:
	.4byte 0x00003fff
.L_08025bac:
	ldr r1, [sp, #48]
	mov r3, r8
	ldrb r2, [r3, #9]
	movs r0, #58
	ldrsh r3, [r1, r0]
	cmp r2, r3
	ble .L_08025bc2
	movs r0, #2
	bl UiWork_SetParamNibble
	b .L_08025bd4
.L_08025bc2:
	ldr r2, [sp, #48]
	ldr r0, .L_08025ce4
	adds r3, r2, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08025bd4
	movs r0, #9
	bl UiWork_SetParamNibble
.L_08025bd4:
	ldr r1, [sp, #64]
	ldr r2, .L_08025ce8
	ldr r0, .L_08025cec
	adds r6, r1, r2
	adds r0, r5, r0
	movs r3, #5
	lsls r5, r7, #4
	strb r3, [r6]
	mov r1, r11
	movs r2, #16
	adds r3, r5, #0
	bl UiText_DrawCharacterAtOffset
	mov r3, r8
	ldrb r0, [r3, #9]
	movs r1, #2
	movs r3, #104
	mov r2, r11
	str r5, [sp, #0]
	bl UiText_DrawNumberAtOffset
	movs r0, #15
	bl UiWork_SetParamNibble
	movs r3, #15
	mov r0, r8
	strb r3, [r6]
	ldrb r3, [r0, #2]
	cmp r3, #4
	beq .L_08025c24
	ldr r2, .L_08025cf0
	adds r1, r3, #0
	movs r3, #0
	adds r1, r1, r2
	str r3, [sp, #0]
	mov r0, r11
	movs r2, #15
	mov r3, r9
	bl UiWindow_SetTilemapEntry
.L_08025c24:
	mov r0, r8
	ldrb r3, [r0, #8]
	cmp r3, #255
	bne .L_08025c30
	movs r3, #11
	b .L_08025c32
.L_08025c30:
	subs r3, #1
.L_08025c32:
	movs r1, #0
	str r1, [sp, #0]
	mov r0, r11
	movs r1, #16
	mov r2, r9
	adds r7, #1
	bl UiWindow_DrawThreeTileColumn
	cmp r7, #4
	bgt .L_08025c56
	ldr r2, [sp, #60]
	ldr r0, [sp, #72]
	adds r3, r2, r7
	lsls r3, r3, #1
	ldrh r5, [r3, r0]
	cmp r5, #0
	beq .L_08025c56
	b .L_08025b24
.L_08025c56:
	ldr r1, [sp, #60]
	str r7, [sp, #40]
	mov r9, r1
.L_08025c5c:
	ldr r2, [sp, #68]
	cmp r2, #5
	ble .L_08025ca6
	movs r7, #0
	adds r2, #4
	mov r8, r2
	b .L_08025c98
.L_08025c6a:
	ldr r3, .L_08025cf4
	ldr r0, [sp, #60]
	movs r1, #5
	adds r6, r7, r3
	bl __divsi3
	cmp r7, r0
	bne .L_08025c7e
	ldr r0, .L_08025cf8
	adds r6, r7, r0
.L_08025c7e:
	mov r1, r11
	ldrh r2, [r1, #8]
	subs r2, r2, r5
	adds r2, r2, r7
	movs r3, #0
	str r3, [sp, #0]
	subs r2, #2
	mov r0, r11
	adds r1, r6, #0
	subs r3, #1
	bl UiWindow_SetTilemapEntry
	adds r7, #1
.L_08025c98:
	mov r0, r8
	movs r1, #5
	bl __divsi3
	adds r5, r0, #0
	cmp r7, r5
	blt .L_08025c6a
.L_08025ca6:
	mov r1, r11
	ldrh r0, [r1, #12]
	ldr r2, [sp, #20]
	ldrh r1, [r1, #14]
	mov r3, r11
	adds r1, r1, r2
	ldrh r2, [r3, #8]
	movs r3, #14
	adds r0, #1
	adds r1, #1
	subs r2, #2
	str r3, [sp, #0]
	movs r3, #1
	bl Ui_SetRectHighlight
	ldr r1, .L_08025cfc
	ldr r0, [sp, #64]
	movs r3, #1
	adds r2, r0, r1
	strb r3, [r2]
	ldr r2, .L_08025d00
	adds r3, r0, r2
	movs r0, #0
	strb r0, [r3]
.L_08025cd6:
	ldr r1, [sp, #68]
	cmp r1, #5
	ble .L_08025d98
	movs r7, #0
	adds r1, #4
	mov r8, r1
	b .L_08025d48
.L_08025ce4:
	.4byte 0x0000013d
.L_08025ce8:
	.4byte 0x00000ea7
.L_08025cec:
	.4byte 0x00000333
.L_08025cf0:
	.4byte 0x00005001
.L_08025cf4:
	.4byte 0x0000f301
.L_08025cf8:
	.4byte 0x0000f30b
.L_08025cfc:
	.4byte 0x00000ea3
.L_08025d00:
	.4byte 0x00000ea6
.L_08025d04:
	ldr r0, .L_08025e20
	ldr r2, .L_08025e24
	ldr r3, [r0]
	adds r6, r7, r2
	movs r2, #15
	ands r3, r2
	cmp r3, #11
	bhi .L_08025d24
	ldr r0, [sp, #60]
	movs r1, #5
	bl __divsi3
	cmp r7, r0
	bne .L_08025d24
	ldr r1, .L_08025e28
	adds r6, r7, r1
.L_08025d24:
	mov r2, r11
	movs r1, #5
	mov r0, r8
	ldrh r5, [r2, #8]
	bl __divsi3
	subs r5, r5, r0
	adds r5, r5, r7
	movs r3, #0
	subs r5, #2
	str r3, [sp, #0]
	mov r0, r11
	adds r1, r6, #0
	adds r2, r5, #0
	subs r3, #1
	bl UiWindow_SetTilemapEntry
	adds r7, #1
.L_08025d48:
	mov r0, r8
	movs r1, #5
	bl __divsi3
	cmp r7, r0
	blt .L_08025d04
	mov r1, r11
	ldrh r2, [r1, #8]
	movs r5, #1
	negs r5, r5
	subs r2, r2, r0
	movs r3, #0
	str r3, [sp, #0]
	mov r0, r11
	adds r3, r5, #0
	subs r2, #3
	ldr r1, .L_08025e2c
	bl UiWindow_SetTilemapEntry
	mov r0, r11
	ldrh r2, [r0, #8]
	movs r1, #0
	str r1, [sp, #0]
	subs r2, #2
	ldr r1, .L_08025e30
	adds r3, r5, #0
	bl UiWindow_SetTilemapEntry
	ldr r2, [sp, #64]
	ldr r3, .L_08025e34
	mov r0, r11
	adds r1, r2, r3
	ldrh r3, [r0, #14]
	subs r3, #1
	lsrs r3, r3, #2
	movs r2, #2
	lsls r2, r3
	ldrb r3, [r1]
	orrs r2, r3
	strb r2, [r1]
.L_08025d98:
	ldr r1, [sp, #40]
	cmp r1, #0
	ble .L_08025db2
	ldr r5, [sp, #12]
	adds r7, r1, #0
.L_08025da2:
	adds r0, r5, #0
	movs r1, #240
	subs r7, #1
	bl Runtime_PushSlotEntry
	adds r5, #12
	cmp r7, #0
	bne .L_08025da2
.L_08025db2:
	mov r2, r11
	ldrh r3, [r2, #12]
	lsls r3, r3, #3
	subs r3, #4
	ldr r0, [sp, #20]
	str r3, [sp, #28]
	ldrh r3, [r2, #14]
	adds r3, r0, r3
	lsls r3, r3, #3
	adds r3, #20
	ldr r1, [sp, #24]
	str r3, [sp, #32]
	movs r3, #128
	lsls r3, r3, #23
	movs r2, #0
	str r3, [r1, #4]
	str r2, [r1, #8]
	ldr r0, [sp, #52]
	ldr r1, .L_08025e38
	bl Resource_GetBuffer
	ldr r3, .L_08025e10
	ldr r1, [sp, #24]
	ands r0, r3
	ldr r2, .L_08025e14
	ldrh r3, [r1, #8]
	ands r3, r2
	orrs r3, r0
	adds r2, r1, #0
	strh r3, [r2, #8]
	ldr r3, .L_08025e20
	ldr r2, [r3]
	movs r0, #4
	ands r2, r0
	ldr r1, [sp, #28]
	ldr r3, .L_08025e3c
	lsrs r2, r2, #1
	adds r2, r1, r2
	adds r2, r2, r3
	ldr r1, [sp, #24]
	ldr r3, .L_08025e18
	ands r2, r3
	ldrh r3, [r1, #6]
	ldr r1, .L_08025e1c
	ands r3, r1
	b .L_08025e40
	.2byte 0x0000
.L_08025e10:
	.4byte 0x000003ff
.L_08025e14:
	.4byte 0xfffffc00
.L_08025e18:
	.4byte 0x000001ff
.L_08025e1c:
	.4byte 0xfffffe00
.L_08025e20:
	.4byte gFrameCount
.L_08025e24:
	.4byte 0x0000f301
.L_08025e28:
	.4byte 0x0000f30b
.L_08025e2c:
	.4byte 0x0000f334
.L_08025e30:
	.4byte 0x0000f335
.L_08025e34:
	.4byte 0x00000ea3
.L_08025e38:
	.4byte Resource_FixedBlockBTiles
.L_08025e3c:
	.4byte 0x0000fffc
.L_08025e40:
	orrs r3, r2
	ldr r1, .L_08026070
	ldr r2, [sp, #24]
	strh r3, [r2, #6]
	ldr r3, [r1]
	ldr r2, [sp, #32]
	ands r3, r0
	lsrs r3, r3, #2
	subs r3, r2, r3
	ldr r0, [sp, #24]
	adds r3, #248
	strb r3, [r0, #4]
	ldr r1, [sp, #68]
	cmp r1, #0
	beq .L_08025e66
	ldr r0, [sp, #24]
	movs r1, #242
	bl Runtime_PushSlotEntry
.L_08025e66:
	ldr r3, .L_08026074
	ldr r2, [sp, #60]
	ldr r1, [r3]
	mov r3, r10
	str r2, [r1, #52]
	str r3, [r1, #48]
	ldr r0, [sp, #36]
	str r0, [r1, #56]
	ldr r0, .L_08026078
	ldr r3, [r0]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08025eaa
	ldr r1, [sp, #68]
	cmp r1, #0
	beq .L_08025ea4
	ldr r6, [sp, #60]
	ldr r2, [sp, #72]
	add r6, r10
	lsls r3, r6, #1
	ldrh r0, [r3, r2]
	bl Ability_GetData
	ldrb r2, [r0, #1]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_08025ea2
	b .L_0802602a
.L_08025ea2:
	b .L_08025ec6
.L_08025ea4:
	movs r6, #1
	negs r6, r6
	b .L_0802602a
.L_08025eaa:
	ldr r3, [r1, #76]
	cmp r3, #0
	beq .L_08025eba
	ldr r3, [r0]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08025ec6
.L_08025eba:
	movs r0, #113
	movs r6, #1
	bl AudioCommand_PlayFar
	negs r6, r6
	b .L_0802602a
.L_08025ec6:
	ldr r3, [sp, #68]
	cmp r3, #0
	bne .L_08025ece
	b .L_08026022
.L_08025ece:
	ldr r1, .L_0802607c
	ldr r3, [r1]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_08025f04
	movs r0, #111
	bl AudioCommand_PlayFar
	movs r0, #1
	add r10, r0
	mov r1, r10
	cmp r1, #5
	beq .L_08025ef4
	ldr r3, [sp, #60]
	ldr r2, [sp, #68]
	add r3, r10
	cmp r3, r2
	bne .L_08025ef8
.L_08025ef4:
	movs r3, #0
	mov r10, r3
.L_08025ef8:
	mov r1, r10
	mov r0, r10
	lsls r1, r1, #1
	str r0, [sp, #36]
	str r1, [sp, #20]
	b .L_08026022
.L_08025f04:
	ldr r3, [r1]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_08025f4e
	movs r0, #111
	bl AudioCommand_PlayFar
	movs r2, #1
	negs r2, r2
	add r10, r2
	mov r3, r10
	cmp r3, #0
	bge .L_08025f42
	ldr r0, [sp, #68]
	movs r1, #5
	subs r0, #1
	bl __divsi3
	lsls r3, r0, #2
	adds r3, r3, r0
	ldr r0, [sp, #60]
	cmp r0, r3
	bne .L_08025f3e
	ldr r1, [sp, #68]
	subs r3, r1, r0
	subs r3, #1
	mov r10, r3
	b .L_08025f42
.L_08025f3e:
	movs r2, #4
	mov r10, r2
.L_08025f42:
	mov r0, r10
	mov r3, r10
	lsls r0, r0, #1
	str r3, [sp, #36]
	str r0, [sp, #20]
	b .L_08026022
.L_08025f4e:
	ldr r3, [r1]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_08025fae
	movs r0, #111
	bl AudioCommand_PlayFar
	bl Runtime_SetMainState19
	ldr r3, [sp, #60]
	ldr r1, [sp, #68]
	adds r3, #5
	cmp r3, r1
	blt .L_08025f82
	ldr r2, [sp, #60]
	cmp r2, #0
	beq .L_08026022
	ldr r0, [sp, #36]
	mov r10, r0
	mov r1, r10
	movs r3, #0
	lsls r1, r1, #1
	str r3, [sp, #60]
	str r1, [sp, #20]
	b .L_08026022
.L_08025f82:
	ldr r0, [sp, #68]
	ldr r2, [sp, #36]
	subs r0, #1
	movs r1, #5
	str r3, [sp, #60]
	mov r10, r2
	bl __divsi3
	lsls r3, r0, #2
	adds r3, r3, r0
	ldr r0, [sp, #60]
	cmp r0, r3
	bne .L_08026004
	ldr r1, [sp, #68]
	subs r3, r1, r0
	subs r3, #1
	ldr r2, [sp, #36]
	mov r10, r3
	cmp r10, r2
	ble .L_0802600c
	mov r10, r2
	b .L_0802601c
.L_08025fae:
	ldr r3, [r1]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_08026022
	movs r0, #111
	bl AudioCommand_PlayFar
	bl Runtime_SetMainState19
	ldr r0, [sp, #60]
	cmp r0, #0
	beq .L_08025fd8
	ldr r1, [sp, #36]
	mov r10, r1
	mov r2, r10
	subs r0, #5
	lsls r2, r2, #1
	str r0, [sp, #60]
	str r2, [sp, #20]
	b .L_08026022
.L_08025fd8:
	ldr r0, [sp, #68]
	movs r1, #5
	subs r0, #1
	bl __divsi3
	lsls r3, r0, #2
	adds r3, r3, r0
	str r3, [sp, #60]
	ldr r0, [sp, #60]
	ldr r3, [sp, #36]
	mov r10, r3
	cmp r0, #0
	beq .L_08026014
	ldr r1, [sp, #68]
	subs r3, r1, r0
	subs r3, #1
	ldr r2, [sp, #36]
	mov r10, r3
	cmp r10, r2
	ble .L_0802601c
	mov r10, r2
	b .L_0802601c
.L_08026004:
	mov r0, r10
	lsls r0, r0, #1
	str r0, [sp, #20]
	b .L_08026022
.L_0802600c:
	mov r1, r10
	lsls r1, r1, #1
	str r1, [sp, #20]
	b .L_08026022
.L_08026014:
	mov r2, r10
	lsls r2, r2, #1
	str r2, [sp, #20]
	b .L_08026022
.L_0802601c:
	mov r3, r10
	lsls r3, r3, #1
	str r3, [sp, #20]
.L_08026022:
	movs r0, #1
	bl WaitFrames
	b .L_08025a72
.L_0802602a:
	ldr r0, [sp, #44]
	movs r1, #1
	bl UiWork_Finalize
	mov r0, r11
	movs r1, #1
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
	ldr r5, [sp, #8]
	movs r7, #4
.L_08026044:
	ldmia r5!, {r0}
	subs r7, #1
	bl Resource_ResetEntry
	cmp r7, #0
	bge .L_08026044
	ldr r0, [sp, #52]
	bl Resource_ResetEntry
	movs r0, #1
	bl WaitFrames
	adds r0, r6, #0
	add sp, #300
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_08026070:
	.4byte gFrameCount
.L_08026074:
	.4byte gLinkCountdownWork
.L_08026078:
	.4byte gKeyState
.L_0802607c:
	.4byte gKeysRepeat
