.syntax unified
	.thumb
	.global SummonMenu_SelectSummon
	.thumb_func
SummonMenu_SelectSummon:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #372
	str r2, [sp, #84]
	ldr r5, .L_08024a00
	ldr r0, [r5]
	movs r1, #1
	str r0, [sp, #72]
	negs r1, r1
	movs r0, #128
	str r1, [sp, #68]
	mov r9, r1
	bl Resource_LoadIntoFreeSlot
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #42
	str r0, [sp, #64]
	str r3, [sp, #0]
	movs r1, #4
	movs r2, #30
	movs r3, #4
	movs r0, #0
	bl UiWindow_Create
	movs r6, #6
	str r0, [sp, #60]
	movs r1, #8
	movs r2, #10
	movs r3, #3
	movs r0, #20
	str r6, [sp, #0]
	bl UiWindow_Create
	movs r2, #0
	str r0, [sp, #56]
	str r2, [sp, #52]
	adds r5, #168
	ldr r3, [r5]
	ldr r0, [r3, #52]
	ldr r1, [r3, #48]
	ldr r3, [r3, #56]
	mov r11, r0
	mov r10, r1
	str r3, [sp, #48]
	str r6, [sp, #0]
	movs r2, #17
	movs r3, #9
	movs r0, #13
	movs r1, #11
	bl UiWindow_Create
	movs r2, #156
	lsls r2, r2, #1
	add r2, sp
	ldr r3, .L_08024a04
	movs r7, #128
	str r0, [sp, #76]
	str r2, [sp, #28]
	movs r4, #0
	mov r12, r3
	adds r5, r2, #0
	lsls r7, r7, #23
	movs r6, #0
.L_080249be:
	lsls r0, r4, #1
	str r7, [r5, #4]
	str r6, [r5, #8]
	ldr r1, [sp, #76]
	ldrh r2, [r1, #12]
	ldr r3, .L_080249fc
	lsls r2, r2, #3
	ldrh r1, [r5, #6]
	adds r2, #8
	ands r2, r3
	mov r3, r12
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #6]
	ldr r2, [sp, #76]
	ldrh r3, [r2, #14]
	adds r0, r0, r3
	lsls r0, r0, #3
	adds r0, #4
	adds r4, #1
	strb r0, [r5, #4]
	adds r5, #12
	cmp r4, #3
	ble .L_080249be
	ldr r3, .L_08024a08
	ldr r7, [sp, #28]
	movs r5, #8
	add r6, sp, #96
	mov r8, r3
	movs r4, #3
	b .L_08024a0c
.L_080249fc:
	.4byte 0x000001ff
.L_08024a00:
	.4byte gWindowWork
.L_08024a04:
	.4byte 0xfffffe00
.L_08024a08:
	.4byte 0xfffffc00
.L_08024a0c:
	movs r0, #128
	str r4, [sp, #4]
	bl Resource_LoadIntoFreeSlot
	movs r1, #1
	negs r1, r1
	stmia r6!, {r0}
	bl Resource_GetBuffer
	ldr r3, .L_08024a38
	ands r0, r3
	ldrh r3, [r5, r7]
	mov r1, r8
	ldr r4, [sp, #4]
	ands r3, r1
	orrs r3, r0
	subs r4, #1
	strh r3, [r5, r7]
	adds r5, #12
	cmp r4, #0
	bge .L_08024a0c
	b .L_08024a3c
.L_08024a38:
	.4byte 0x000003ff
.L_08024a3c:
	movs r2, #138
	lsls r2, r2, #1
	add r2, sp
	mov r8, r2
	mov r0, r8
	bl Trade_ListFlaggedEntriesFar
	str r0, [sp, #80]
	movs r7, #0
	adds r3, r0, #0
	subs r3, #1
	str r3, [sp, #20]
	cmp r3, #0
	blt .L_08024aa0
	mov r0, sp
	adds r0, #240
	adds r5, r3, #0
	str r0, [sp, #32]
	add r5, r8
.L_08024a62:
	ldrb r6, [r5]
	adds r0, r6, #0
	bl SummonDefinition_Get
	ldr r1, [sp, #84]
	adds r0, #4
	ldrb r2, [r0]
	ldrb r3, [r1]
	movs r4, #0
	cmp r2, r3
	bhi .L_08024a8a
.L_08024a78:
	adds r4, #1
	cmp r4, #3
	bgt .L_08024a8a
	adds r0, #1
	adds r1, #1
	ldrb r2, [r0]
	ldrb r3, [r1]
	cmp r2, r3
	bls .L_08024a78
.L_08024a8a:
	cmp r4, #4
	bne .L_08024a98
	ldr r2, [sp, #32]
	movs r3, #32
	strb r6, [r2, r7]
	strb r3, [r5]
	adds r7, #1
.L_08024a98:
	subs r5, #1
	cmp r5, r8
	bge .L_08024a62
	b .L_08024aa6
.L_08024aa0:
	mov r3, sp
	adds r3, #240
	str r3, [sp, #32]
.L_08024aa6:
	ldr r0, [sp, #80]
	cmp r0, #0
	ble .L_08024ac8
	ldr r2, [sp, #32]
	adds r1, r7, r2
	ldr r2, [sp, #80]
	mov r0, r8
.L_08024ab4:
	ldrb r3, [r0]
	adds r0, #1
	cmp r3, #32
	beq .L_08024ac2
	strb r3, [r1]
	adds r7, #1
	adds r1, #1
.L_08024ac2:
	subs r2, #1
	cmp r2, #0
	bne .L_08024ab4
.L_08024ac8:
	ldr r0, [sp, #32]
	movs r3, #32
	strb r3, [r0, r7]
	movs r1, #180
	ldr r3, [sp, #64]
	lsls r1, r1, #1
	mov r2, r10
	add r1, sp
	lsls r2, r2, #1
	lsls r3, r3, #16
	str r1, [sp, #24]
	str r2, [sp, #16]
	str r3, [sp, #12]
.L_08024ae2:
	cmp r11, r9
	bne .L_08024aee
	ldr r0, [sp, #68]
	cmp r10, r0
	bne .L_08024aee
	b .L_08024d90
.L_08024aee:
	ldr r1, [sp, #72]
	ldr r2, .L_08024c30
	movs r0, #1
	adds r3, r1, r2
	strb r0, [r3]
	ldr r1, [sp, #76]
	ldr r2, [sp, #68]
	ldrh r0, [r1, #12]
	ldrh r1, [r1, #14]
	lsls r3, r2, #1
	adds r1, r1, r3
	ldr r3, [sp, #76]
	ldrh r2, [r3, #8]
	movs r3, #15
	str r3, [sp, #0]
	subs r2, #2
	adds r1, #1
	movs r3, #1
	adds r0, #1
	bl Ui_SetRectHighlight
	bl Ui_FillVramBlockPattern
	mov r3, r11
	ldr r1, [sp, #32]
	add r3, r10
	ldrb r0, [r1, r3]
	bl SummonDefinition_Get
	adds r6, r0, #0
	ldrh r0, [r6]
	ldr r3, .L_08024c34
	add r5, sp, #112
	adds r0, r0, r3
	adds r1, r5, #0
	movs r2, #52
	bl UiText_CopyMessageString
	movs r2, #0
	ldr r1, [sp, #60]
	movs r3, #4
	adds r0, r5, #0
	bl UiText_RenderWideStringAtOffset
	movs r3, #0
	mov r2, r10
	str r3, [sp, #52]
	str r2, [sp, #68]
	movs r1, #1
	movs r2, #0
	adds r6, #4
.L_08024b54:
	ldrb r3, [r6]
	adds r6, #1
	cmp r3, #0
	beq .L_08024b66
	ldr r0, [sp, #52]
	adds r3, r1, #0
	lsls r3, r2
	orrs r0, r3
	str r0, [sp, #52]
.L_08024b66:
	adds r2, #1
	cmp r2, #3
	ble .L_08024b54
	cmp r11, r9
	bne .L_08024b72
	b .L_08024d06
.L_08024b72:
	ldr r0, [sp, #76]
	bl RenderOutput_RedrawSavedRect
	movs r5, #0
	movs r7, #0
	movs r6, #1
.L_08024b7e:
	ldr r2, .L_08024c38
	ldr r0, [sp, #56]
	adds r1, r5, r2
	movs r3, #0
	lsls r2, r5, #1
	str r7, [sp, #0]
	bl UiWindow_SetTilemapEntry
	ldr r3, [sp, #84]
	ldrb r1, [r3, r5]
	adds r2, r6, #0
	adds r1, #48
	ldr r0, [sp, #56]
	movs r3, #0
	adds r5, #1
	str r7, [sp, #0]
	adds r6, #2
	bl UiWindow_PutGlyph
	cmp r5, #3
	ble .L_08024b7e
	ldr r0, [sp, #32]
	mov r1, r11
	ldrb r6, [r0, r1]
	movs r4, #0
	cmp r6, #32
	bne .L_08024bb6
	b .L_08024ce8
.L_08024bb6:
	mov r2, sp
	adds r2, #88
	str r2, [sp, #8]
.L_08024bbc:
	adds r0, r6, #0
	str r4, [sp, #4]
	bl SummonDefinition_Get
	str r0, [sp, #36]
	adds r1, r0, #0
	ldr r0, [sp, #84]
	adds r1, #4
	ldrb r2, [r1]
	ldrb r3, [r0]
	movs r7, #0
	ldr r4, [sp, #4]
	cmp r2, r3
	bhi .L_08024bea
.L_08024bd8:
	adds r7, #1
	cmp r7, #3
	bgt .L_08024bea
	adds r1, #1
	adds r0, #1
	ldrb r2, [r1]
	ldrb r3, [r0]
	cmp r2, r3
	bls .L_08024bd8
.L_08024bea:
	movs r3, #4
	eors r3, r7
	ldr r2, [sp, #36]
	negs r5, r3
	orrs r5, r3
	ldr r0, .L_08024c28
	ldrh r3, [r2]
	movs r1, #1
	ands r0, r3
	add r2, sp, #96
	lsls r3, r4, #2
	lsrs r5, r5, #31
	adds r2, r2, r3
	str r1, [sp, #0]
	ldr r3, [sp, #8]
	subs r5, r1, r5
	movs r1, #0
	str r4, [sp, #4]
	bl Ability_LoadGlyph
	ldr r4, [sp, #4]
	lsls r3, r4, #1
	adds r1, r3, r4
	ldr r2, [sp, #28]
	mov r8, r3
	lsls r1, r1, #2
	ldr r3, .L_08024c2c
	ldr r0, [sp, #88]
	adds r1, #8
	ands r0, r3
	b .L_08024c3c
.L_08024c28:
	.4byte 0x00003fff
.L_08024c2c:
	.4byte 0x000003ff
.L_08024c30:
	.4byte 0x00000ea6
.L_08024c34:
	.4byte 0x0000053a
.L_08024c38:
	.4byte 0x00005001
.L_08024c3c:
	ldrh r3, [r2, r1]
	ldr r2, .L_08024c78
	ands r3, r2
	orrs r3, r0
	ldr r0, [sp, #28]
	strh r3, [r0, r1]
	cmp r5, #0
	bne .L_08024c54
	movs r0, #2
	bl UiWork_SetParamNibble
	ldr r4, [sp, #4]
.L_08024c54:
	adds r0, r6, #0
	str r4, [sp, #4]
	bl SummonDefinition_Get
	ldr r3, .L_08024c7c
	ldr r4, [sp, #4]
	ldrh r0, [r0]
	ldr r1, [sp, #76]
	adds r0, r0, r3
	movs r2, #16
	lsls r3, r4, #4
	bl UiText_DrawCharacterAtOffset
	movs r1, #0
	ldr r6, [sp, #36]
	lsls r3, r1, #1
	b .L_08024c80
	.2byte 0x0000
.L_08024c78:
	.4byte 0xfffffc00
.L_08024c7c:
	.4byte 0x00000333
.L_08024c80:
	adds r5, r3, #0
	ldr r4, [sp, #4]
	movs r7, #0
	mov r9, r1
	adds r6, #4
	adds r5, #13
.L_08024c8c:
	ldrb r3, [r6]
	cmp r3, #0
	beq .L_08024cbc
	ldr r2, .L_08024d14
	mov r3, r9
	adds r1, r7, r2
	str r3, [sp, #0]
	ldr r0, [sp, #76]
	adds r2, r5, #0
	mov r3, r8
	str r4, [sp, #4]
	bl UiWindow_SetTilemapEntry
	ldrb r1, [r6]
	mov r0, r9
	adds r2, r5, #1
	str r0, [sp, #0]
	adds r1, #48
	ldr r0, [sp, #76]
	mov r3, r8
	bl UiWindow_PutGlyph
	ldr r4, [sp, #4]
	adds r5, #2
.L_08024cbc:
	adds r7, #1
	adds r6, #1
	cmp r7, #3
	ble .L_08024c8c
	movs r0, #15
	str r4, [sp, #4]
	bl UiWork_SetParamNibble
	ldr r4, [sp, #4]
	add r3, sp, #92
	movs r1, #1
	strb r1, [r3, r4]
	adds r4, #1
	cmp r4, #3
	bgt .L_08024d04
	mov r2, r11
	ldr r0, [sp, #32]
	adds r3, r2, r4
	ldrb r6, [r0, r3]
	cmp r6, #32
	beq .L_08024ce8
	b .L_08024bbc
.L_08024ce8:
	cmp r4, #3
	bgt .L_08024d04
	add r2, sp, #372
	ldr r0, .L_08024d18
	adds r3, r4, r2
	adds r2, r3, r0
	movs r3, #4
	movs r1, #0
	subs r4, r3, r4
.L_08024cfa:
	subs r4, #1
	strb r1, [r2]
	adds r2, #1
	cmp r4, #0
	bne .L_08024cfa
.L_08024d04:
	mov r9, r11
.L_08024d06:
	ldr r1, [sp, #80]
	cmp r1, #4
	ble .L_08024d60
	movs r4, #0
	adds r5, r1, #0
	adds r5, #3
	b .L_08024d50
.L_08024d14:
	.4byte 0x00005001
.L_08024d18:
	.4byte 0xfffffee8
.L_08024d1c:
	ldr r2, .L_08024e20
	mov r3, r11
	adds r1, r4, r2
	cmp r3, #0
	bge .L_08024d28
	adds r3, #3
.L_08024d28:
	asrs r3, r3, #2
	cmp r4, r3
	bne .L_08024d32
	ldr r3, .L_08024e24
	adds r1, r4, r3
.L_08024d32:
	ldr r3, [sp, #76]
	ldrh r2, [r3, #8]
	subs r2, r2, r0
	movs r0, #0
	adds r2, r2, r4
	str r0, [sp, #0]
	adds r0, r3, #0
	movs r3, #1
	subs r2, #2
	negs r3, r3
	str r4, [sp, #4]
	bl UiWindow_SetTilemapEntry
	ldr r4, [sp, #4]
	adds r4, #1
.L_08024d50:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_08024d5a
	ldr r3, [sp, #80]
	adds r3, #6
.L_08024d5a:
	asrs r0, r3, #2
	cmp r4, r0
	blt .L_08024d1c
.L_08024d60:
	ldr r1, [sp, #76]
	ldr r2, [sp, #16]
	ldrh r0, [r1, #12]
	ldr r3, [sp, #76]
	ldrh r1, [r1, #14]
	adds r1, r1, r2
	ldrh r2, [r3, #8]
	movs r3, #14
	adds r0, #1
	adds r1, #1
	subs r2, #2
	str r3, [sp, #0]
	movs r3, #1
	bl Ui_SetRectHighlight
	ldr r1, .L_08024e28
	ldr r0, [sp, #72]
	movs r2, #1
	adds r3, r0, r1
	adds r1, #3
	strb r2, [r3]
	adds r3, r0, r1
	movs r2, #0
	strb r2, [r3]
.L_08024d90:
	ldr r6, [sp, #28]
	movs r4, #0
	add r5, sp, #92
.L_08024d96:
	ldrb r3, [r5]
	adds r5, #1
	cmp r3, #0
	beq .L_08024daa
	adds r0, r6, #0
	movs r1, #240
	str r4, [sp, #4]
	bl Runtime_PushSlotEntry
	ldr r4, [sp, #4]
.L_08024daa:
	adds r4, #1
	adds r6, #12
	cmp r4, #3
	ble .L_08024d96
	ldr r0, [sp, #76]
	ldrh r3, [r0, #12]
	lsls r3, r3, #3
	subs r3, #2
	ldr r1, [sp, #16]
	str r3, [sp, #40]
	ldrh r3, [r0, #14]
	adds r3, r1, r3
	lsls r3, r3, #3
	adds r3, #20
	ldr r2, [sp, #24]
	str r3, [sp, #44]
	movs r3, #128
	lsls r3, r3, #23
	str r3, [r2, #4]
	movs r3, #0
	str r3, [r2, #8]
	ldr r1, [sp, #12]
	lsrs r0, r1, #16
	ldr r1, .L_08024e2c
	bl Resource_GetBuffer
	ldr r3, .L_08024e10
	ldr r2, [sp, #24]
	ands r0, r3
	ldrh r3, [r2, #8]
	ldr r2, .L_08024e14
	ldr r1, .L_08024e30
	ands r3, r2
	orrs r3, r0
	ldr r0, [sp, #24]
	ldr r2, [r1]
	strh r3, [r0, #8]
	movs r0, #4
	ldr r3, [sp, #40]
	ands r2, r0
	ldr r1, .L_08024e34
	lsrs r2, r2, #1
	adds r2, r3, r2
	adds r2, r2, r1
	ldr r3, .L_08024e18
	ldr r1, [sp, #24]
	ands r2, r3
	ldrh r3, [r1, #6]
	ldr r1, .L_08024e1c
	ands r3, r1
	b .L_08024e38
.L_08024e10:
	.4byte 0x000003ff
.L_08024e14:
	.4byte 0xfffffc00
.L_08024e18:
	.4byte 0x000001ff
.L_08024e1c:
	.4byte 0xfffffe00
.L_08024e20:
	.4byte 0x0000f301
.L_08024e24:
	.4byte 0x0000f30b
.L_08024e28:
	.4byte 0x00000ea3
.L_08024e2c:
	.4byte Resource_FixedBlockBTiles
.L_08024e30:
	.4byte gFrameCount
.L_08024e34:
	.4byte 0x0000fffc
.L_08024e38:
	orrs r3, r2
	ldr r1, .L_0802515c
	ldr r2, [sp, #24]
	strh r3, [r2, #6]
	ldr r3, [r1]
	ldr r2, [sp, #44]
	ands r3, r0
	lsrs r3, r3, #2
	ldr r0, [sp, #24]
	subs r3, r2, r3
	adds r3, #248
	strb r3, [r0, #4]
	movs r1, #242
	ldr r0, [sp, #24]
	bl Runtime_PushSlotEntry
	ldr r1, .L_0802515c
	ldr r6, [r1]
	movs r3, #8
	ands r6, r3
	movs r5, #0
.L_08024e62:
	negs r3, r6
	orrs r3, r6
	lsrs r3, r3, #31
	adds r2, r3, #0
	movs r3, #15
	subs r2, r3, r2
	ldr r0, [sp, #52]
	movs r3, #1
	lsls r3, r5
	ands r3, r0
	cmp r3, #0
	bne .L_08024e7c
	movs r2, #15
.L_08024e7c:
	ldr r1, [sp, #56]
	ldrh r0, [r1, #12]
	lsls r3, r5, #1
	ldrh r1, [r1, #14]
	adds r0, r0, r3
	str r2, [sp, #0]
	adds r0, #1
	adds r1, #1
	movs r2, #2
	movs r3, #1
	adds r5, #1
	bl Ui_SetRectHighlight
	cmp r5, #3
	ble .L_08024e62
	ldr r2, [sp, #80]
	cmp r2, #4
	ble .L_08024f48
	movs r4, #0
	adds r5, r2, #0
	adds r5, #3
	b .L_08024ef2
.L_08024ea8:
	ldr r3, .L_08025160
	ldr r0, .L_0802515c
	adds r1, r4, r3
	ldr r3, [r0]
	movs r2, #15
	ands r3, r2
	cmp r3, #11
	bhi .L_08024eca
	mov r3, r11
	cmp r3, #0
	bge .L_08024ec0
	adds r3, #3
.L_08024ec0:
	asrs r3, r3, #2
	cmp r4, r3
	bne .L_08024eca
	ldr r2, .L_08025164
	adds r1, r4, r2
.L_08024eca:
	ldr r0, [sp, #76]
	adds r2, r5, #0
	ldrh r3, [r0, #8]
	cmp r5, #0
	bge .L_08024ed8
	ldr r2, [sp, #80]
	adds r2, #6
.L_08024ed8:
	asrs r2, r2, #2
	subs r2, r3, r2
	adds r2, r2, r4
	movs r3, #0
	str r3, [sp, #0]
	subs r2, #2
	ldr r0, [sp, #76]
	subs r3, #1
	str r4, [sp, #4]
	bl UiWindow_SetTilemapEntry
	ldr r4, [sp, #4]
	adds r4, #1
.L_08024ef2:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_08024efc
	ldr r3, [sp, #80]
	adds r3, #6
.L_08024efc:
	asrs r2, r3, #2
	cmp r4, r2
	blt .L_08024ea8
	ldr r0, [sp, #76]
	ldrh r3, [r0, #8]
	movs r5, #1
	negs r5, r5
	subs r2, r3, r2
	movs r1, #0
	str r1, [sp, #0]
	ldr r0, [sp, #76]
	adds r3, r5, #0
	subs r2, #3
	ldr r1, .L_08025168
	bl UiWindow_SetTilemapEntry
	ldr r3, [sp, #76]
	ldrh r2, [r3, #8]
	movs r0, #0
	str r0, [sp, #0]
	subs r2, #2
	adds r0, r3, #0
	ldr r1, .L_0802516c
	adds r3, r5, #0
	bl UiWindow_SetTilemapEntry
	ldr r2, [sp, #72]
	ldr r3, .L_08025170
	ldr r0, [sp, #76]
	adds r1, r2, r3
	ldrh r3, [r0, #14]
	subs r3, #1
	lsrs r3, r3, #2
	movs r2, #2
	lsls r2, r3
	ldrb r3, [r1]
	orrs r2, r3
	strb r2, [r1]
.L_08024f48:
	ldr r3, .L_08025174
	ldr r2, [r3]
	mov r1, r11
	mov r3, r10
	str r1, [r2, #52]
	str r3, [r2, #48]
	ldr r0, [sp, #48]
	str r0, [r2, #56]
	ldr r3, .L_08025178
	ldr r1, [r3]
	ldr r3, .L_0802517c
	ldr r0, [r3]
	adds r3, r2, #0
	adds r3, #216
	ldr r3, [r3]
	cmp r3, #0
	beq .L_08024f84
	adds r2, #220
	ldr r3, [r2]
	movs r0, #0
	movs r1, #0
	cmp r3, #0
	bne .L_08024f80
	movs r3, #120
	str r3, [r2]
	movs r0, #1
	movs r1, #1
	b .L_08024f84
.L_08024f80:
	subs r3, #1
	str r3, [r2]
.L_08024f84:
	adds r3, r1, #0
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08024f98
	mov r3, r11
	ldr r0, [sp, #32]
	add r3, r10
	ldrb r6, [r0, r3]
	b .L_08025106
.L_08024f98:
	ldr r3, .L_08025174
	ldr r3, [r3]
	ldr r3, [r3, #76]
	cmp r3, #0
	beq .L_08024faa
	movs r3, #2
	ands r3, r1
	cmp r3, #0
	beq .L_08024fb6
.L_08024faa:
	movs r0, #113
	movs r6, #1
	bl AudioCommand_PlayFar
	negs r6, r6
	b .L_08025106
.L_08024fb6:
	movs r3, #128
	ands r3, r0
	cmp r3, #0
	beq .L_08024fe6
	movs r0, #111
	bl AudioCommand_PlayFar
	movs r1, #1
	add r10, r1
	mov r2, r10
	cmp r2, #4
	beq .L_08024fd8
	mov r3, r11
	ldr r0, [sp, #80]
	add r3, r10
	cmp r3, r0
	bne .L_08024fdc
.L_08024fd8:
	movs r1, #0
	mov r10, r1
.L_08024fdc:
	mov r3, r10
	mov r2, r10
	lsls r3, r3, #1
	str r2, [sp, #48]
	b .L_080250fc
.L_08024fe6:
	movs r3, #64
	ands r3, r0
	cmp r3, #0
	beq .L_0802502c
	movs r0, #111
	bl AudioCommand_PlayFar
	movs r0, #1
	negs r0, r0
	add r10, r0
	mov r1, r10
	cmp r1, #0
	bge .L_08025022
	ldr r3, [sp, #20]
	cmp r3, #0
	bge .L_0802500a
	ldr r3, [sp, #80]
	adds r3, #2
.L_0802500a:
	asrs r3, r3, #2
	lsls r3, r3, #2
	cmp r11, r3
	bne .L_0802501e
	ldr r2, [sp, #80]
	mov r0, r11
	subs r3, r2, r0
	subs r3, #1
	mov r10, r3
	b .L_08025022
.L_0802501e:
	movs r1, #3
	mov r10, r1
.L_08025022:
	mov r3, r10
	mov r2, r10
	lsls r3, r3, #1
	str r2, [sp, #48]
	b .L_080250fc
.L_0802502c:
	movs r3, #16
	ands r3, r0
	cmp r3, #0
	beq .L_0802508a
	movs r0, #111
	bl AudioCommand_PlayFar
	bl Runtime_SetMainState19
	mov r3, r11
	ldr r0, [sp, #80]
	adds r3, #4
	cmp r3, r0
	blt .L_0802505e
	mov r1, r11
	cmp r1, #0
	beq .L_080250fe
	ldr r3, [sp, #48]
	mov r10, r3
	mov r0, r10
	movs r2, #0
	lsls r0, r0, #1
	mov r11, r2
	str r0, [sp, #16]
	b .L_080250fe
.L_0802505e:
	mov r11, r3
	ldr r1, [sp, #48]
	ldr r3, [sp, #20]
	mov r10, r1
	cmp r3, #0
	bge .L_0802506e
	ldr r3, [sp, #80]
	adds r3, #2
.L_0802506e:
	asrs r3, r3, #2
	lsls r3, r3, #2
	cmp r11, r3
	bne .L_080250e0
	ldr r2, [sp, #80]
	mov r0, r11
	subs r3, r2, r0
	subs r3, #1
	ldr r1, [sp, #48]
	mov r10, r3
	cmp r10, r1
	ble .L_080250e8
	mov r10, r1
	b .L_080250f0
.L_0802508a:
	movs r3, #32
	ands r3, r0
	cmp r3, #0
	beq .L_080250fe
	movs r0, #111
	bl AudioCommand_PlayFar
	bl Runtime_SetMainState19
	mov r3, r11
	cmp r3, #0
	beq .L_080250b4
	ldr r1, [sp, #48]
	mov r10, r1
	movs r0, #4
	mov r2, r10
	negs r0, r0
	lsls r2, r2, #1
	add r11, r0
	str r2, [sp, #16]
	b .L_080250fe
.L_080250b4:
	ldr r3, [sp, #20]
	cmp r3, #0
	bge .L_080250be
	ldr r3, [sp, #80]
	adds r3, #2
.L_080250be:
	asrs r3, r3, #2
	lsls r3, r3, #2
	mov r11, r3
	ldr r3, [sp, #48]
	mov r0, r11
	mov r10, r3
	cmp r0, #0
	beq .L_080250f0
	ldr r1, [sp, #80]
	subs r3, r1, r0
	subs r3, #1
	ldr r2, [sp, #48]
	mov r10, r3
	cmp r10, r2
	ble .L_080250f8
	mov r10, r2
	b .L_080250f8
.L_080250e0:
	mov r0, r10
	lsls r0, r0, #1
	str r0, [sp, #16]
	b .L_080250fe
.L_080250e8:
	mov r1, r10
	lsls r1, r1, #1
	str r1, [sp, #16]
	b .L_080250fe
.L_080250f0:
	mov r2, r10
	lsls r2, r2, #1
	str r2, [sp, #16]
	b .L_080250fe
.L_080250f8:
	mov r3, r10
	lsls r3, r3, #1
.L_080250fc:
	str r3, [sp, #16]
.L_080250fe:
	movs r0, #1
	bl WaitFrames
	b .L_08024ae2
.L_08025106:
	movs r0, #1
	bl WaitFrames
	movs r4, #3
	add r5, sp, #96
.L_08025110:
	ldmia r5!, {r0}
	str r4, [sp, #4]
	bl Resource_ResetEntry
	ldr r4, [sp, #4]
	subs r4, #1
	cmp r4, #0
	bge .L_08025110
	ldr r1, [sp, #12]
	lsrs r0, r1, #16
	bl Resource_ResetEntry
	movs r1, #1
	ldr r0, [sp, #56]
	bl UiWork_Finalize
	movs r1, #1
	ldr r0, [sp, #60]
	bl UiWork_Finalize
	movs r1, #1
	ldr r0, [sp, #76]
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
	adds r0, r6, #0
	add sp, #372
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_0802515c:
	.4byte gFrameCount
.L_08025160:
	.4byte 0x0000f301
.L_08025164:
	.4byte 0x0000f30b
.L_08025168:
	.4byte 0x0000f334
.L_0802516c:
	.4byte 0x0000f335
.L_08025170:
	.4byte 0x00000ea3
.L_08025174:
	.4byte gLinkCountdownWork
.L_08025178:
	.4byte gKeyState
.L_0802517c:
	.4byte gKeysRepeat
