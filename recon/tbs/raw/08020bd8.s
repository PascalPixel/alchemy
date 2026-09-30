.syntax unified
	.thumb
	.global NameEntry_EditOwnerName
	.thumb_func
NameEntry_EditOwnerName:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #96
	mov r2, sp
	movs r1, #0
	adds r2, #81
	str r1, [sp, #36]
	str r1, [sp, #32]
	str r1, [sp, #28]
	add r6, sp, #80
	str r2, [sp, #24]
	str r0, [sp, #44]
	bl Func_08077008
	ldr r3, .L_08020d54
	str r0, [sp, #20]
	ldr r3, [r3]
	str r3, [sp, #16]
	movs r3, #1
	str r3, [sp, #12]
	mov r9, r3
	bl Ui_LoadWindowGraphics
	movs r5, #2
	movs r1, #6
	movs r2, #24
	movs r3, #9
	movs r0, #3
	str r5, [sp, #0]
	bl UiWindow_Create
	movs r1, #3
	mov r8, r0
	movs r2, #8
	movs r3, #3
	movs r0, #8
	str r5, [sp, #0]
	bl UiWindow_Create
	str r0, [sp, #40]
	ldr r0, [sp, #44]
	bl Localization_LookupEntryId
	movs r2, #3
	movs r3, #1
	movs r1, #0
	bl UiWindow_CreateWithSideObject
	ldr r1, .L_08020d58
	mov r0, r8
	bl UiWindow_CopyTilemapRegion
	movs r3, #7
	str r3, [sp, #0]
	mov r0, r8
	movs r1, #18
	movs r2, #0
	movs r3, #18
	bl UiWindow_DrawDividerLine
	ldr r2, .L_08020d5c
	ldr r1, [sp, #16]
	adds r3, r1, r2
	add r1, sp, #12
	add r2, sp, #36
	ldrb r1, [r1]
	ldrb r2, [r2]
	strb r1, [r3]
	strb r2, [r6]
	mov r0, sp
	ldr r1, [sp, #24]
	ldr r2, [sp, #20]
	adds r0, #94
.L_08020c74:
	ldrb r3, [r2]
	adds r2, #1
	strb r3, [r1]
	adds r1, #1
	cmp r3, #0
	beq .L_08020c8c
	ldr r3, [sp, #32]
	adds r3, #1
	str r3, [sp, #32]
	ldr r3, [sp, #28]
	adds r3, #1
	str r3, [sp, #28]
.L_08020c8c:
	cmp r1, r0
	ble .L_08020c74
	ldr r1, [sp, #24]
	movs r3, #0
	strb r3, [r1, #14]
	ldr r0, [sp, #40]
	ldr r1, [sp, #20]
	bl UiText_DrawPaddedLabel
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	movs r6, #18
	movs r7, #5
	cmp r5, #95
	bgt .L_08020ce2
	ldr r2, .L_08020d60
	movs r1, #128
	bl VramBlock_LoadCached
	movs r1, #128
	movs r3, #0
	lsls r1, r1, #23
	adds r0, r5, #0
	mov r2, r8
	str r3, [sp, #0]
	bl RenderOutput_Create
	add r2, sp, #64
	adds r5, r0, #0
	str r5, [r2]
	mov r3, r8
	mov r11, r2
	ldrh r1, [r3, #12]
	ldrh r2, [r3, #14]
	lsls r1, r1, #3
	lsls r2, r2, #3
	adds r1, #140
	adds r2, #52
	mov r0, r11
	bl Func_080b0038
	b .L_08020ce6
.L_08020ce2:
	add r1, sp, #64
	mov r11, r1
.L_08020ce6:
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	cmp r5, #95
	bgt .L_08020d68
	ldr r2, .L_08020d64
	movs r1, #128
	bl VramBlock_LoadCached
	movs r1, #128
	lsls r1, r1, #23
	movs r3, #0
	adds r0, r5, #0
	mov r2, r8
	str r3, [sp, #0]
	bl RenderOutput_Create
	movs r2, #48
	adds r5, r0, #0
	add r2, sp
	str r5, [r2]
	movs r3, #255
	strb r3, [r5, #15]
	mov r10, r2
	movs r3, #13
	ldrb r2, [r5, #25]
	negs r3, r3
	ands r3, r2
	strb r3, [r5, #25]
	ldr r0, [sp, #24]
	bl UiText_SetRenderString
	adds r1, r0, #0
	adds r1, #70
	mov r0, r10
	movs r2, #22
	bl Func_080b0038
	b .L_08020d6c
.L_08020d34:
	mov r3, r10
	ldr r2, [r3]
	movs r3, #13
	strb r3, [r2, #5]
	ldr r0, [sp, #40]
	bl RenderOutput_PrepareForRedraw
	ldr r0, [sp, #40]
	ldr r1, [sp, #20]
	bl UiText_DrawPaddedLabel
	movs r0, #10
	bl WaitFrames
	b .L_08021034
	.2byte 0x0000
.L_08020d54:
	.4byte Data_03001e8c
.L_08020d58:
	.4byte Data_08073864
.L_08020d5c:
	.4byte 0x00000ea3
.L_08020d60:
	.4byte Resource_FixedBlockBTiles
.L_08020d64:
	.4byte RomBytes_080317e4
.L_08020d68:
	add r1, sp, #48
	mov r10, r1
.L_08020d6c:
	ldr r4, .L_08020da0
	ldr r3, .L_08020da4
	ldr r0, .L_08020da8
	adds r1, r4, #0
	ldr r2, .L_08020dac
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_08020d9c
	strh r3, [r4, #8]
	ldr r2, [sp, #32]
	ldr r3, [sp, #24]
	adds r2, r2, r3
	str r2, [sp, #8]
.L_08020d86:
	movs r5, #1
	cmp r6, #18
	bne .L_08020db0
	cmp r7, #4
	bne .L_08020d92
	movs r5, #3
.L_08020d92:
	cmp r7, #5
	bne .L_08020db0
	movs r5, #3
	b .L_08020db0
	.2byte 0x0000
.L_08020d9c:
	.4byte 0x00006318
.L_08020da0:
	.4byte 0x050001c0
.L_08020da4:
	.4byte 0x040000d4
.L_08020da8:
	.4byte 0x050001e0
.L_08020dac:
	.4byte 0x84000008
.L_08020db0:
	movs r1, #1
	movs r3, #14
	str r1, [sp, #0]
	str r3, [sp, #4]
	adds r1, r6, #0
	adds r2, r7, #0
	adds r3, r5, #0
	mov r0, r8
	bl UiWindow_SetTileAttributeRect
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	movs r3, #15
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r0, r8
	adds r3, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl UiWindow_SetTileAttributeRect
	mov r3, r9
	cmp r3, #0
	beq .L_08020e02
	movs r1, #0
	mov r2, r8
	mov r9, r1
	ldrh r1, [r2, #12]
	ldrh r2, [r2, #14]
	adds r1, r1, r6
	adds r2, r2, r7
	lsls r1, r1, #3
	lsls r2, r2, #3
	subs r1, #7
	adds r2, #15
	mov r0, r11
	movs r3, #3
	bl Shop_SetCursorFar
.L_08020e02:
	ldr r3, [sp, #12]
	cmp r3, #0
	beq .L_08020e20
	movs r1, #0
	ldr r0, [sp, #24]
	str r1, [sp, #12]
	bl UiText_SetRenderString
	adds r1, r0, #0
	adds r1, #70
	mov r0, r10
	movs r2, #22
	movs r3, #3
	bl Shop_SetCursorFar
.L_08020e20:
	mov r0, r11
	bl ShopCursor_AdvanceFar
	mov r0, r10
	bl ShopCursor_MoveTowardTargetFar
	ldr r3, .L_08020e74
	ldr r0, [r3]
	mov r3, r10
	ldr r5, [r3]
	movs r2, #7
	ldr r4, .L_08020e78
	lsrs r0, r0, #1
	ands r0, r2
	ldrsb r3, [r4, r0]
	ldrh r1, [r5, #6]
	adds r1, r1, r3
	ldr r3, .L_08020e6c
	ldr r2, .L_08020e70
	ands r1, r3
	ldrh r3, [r5, #22]
	ands r3, r2
	orrs r3, r1
	adds r0, #5
	movs r1, #7
	ands r0, r1
	strh r3, [r5, #22]
	ldrb r2, [r5, #8]
	ldrb r3, [r4, r0]
	adds r2, r2, r3
	strb r2, [r5, #20]
	ldr r5, .L_08020e7c
	ldr r2, [r5]
	movs r3, #64
	ands r2, r3
	cmp r2, #0
	beq .L_08020eaa
	b .L_08020e80
.L_08020e6c:
	.4byte 0x000001ff
.L_08020e70:
	.4byte 0xfffffe00
.L_08020e74:
	.4byte gFrameTick
.L_08020e78:
	.4byte Data_080371f6
.L_08020e7c:
	.4byte gKeysRepeat
.L_08020e80:
	movs r0, #111
	bl Func_080f9010
	movs r2, #1
	mov r9, r2
	subs r7, #1
	cmp r6, #18
	beq .L_08020e9c
	movs r3, #1
	negs r3, r3
	cmp r7, r3
	bne .L_08020eaa
	movs r7, #5
	b .L_08020eaa
.L_08020e9c:
	movs r3, #3
	eors r3, r7
	negs r2, r3
	orrs r2, r3
	lsrs r7, r2, #31
	movs r3, #5
	subs r7, r3, r7
.L_08020eaa:
	ldr r2, [r5]
	movs r3, #128
	ands r2, r3
	cmp r2, #0
	beq .L_08020ed8
	movs r0, #111
	bl Func_080f9010
	movs r1, #1
	mov r9, r1
	adds r7, #1
	cmp r6, #18
	beq .L_08020ecc
	cmp r7, #6
	bne .L_08020ed8
	movs r7, #0
	b .L_08020ed8
.L_08020ecc:
	movs r2, #6
	eors r2, r7
	negs r3, r2
	orrs r3, r2
	lsrs r7, r3, #31
	adds r7, #4
.L_08020ed8:
	ldr r2, [r5]
	movs r3, #32
	ands r2, r3
	cmp r2, #0
	beq .L_08020f12
	movs r0, #111
	bl Func_080f9010
	movs r3, #1
	movs r2, #1
	subs r6, #1
	negs r3, r3
	mov r9, r2
	cmp r6, r3
	bne .L_08020f02
	subs r3, r7, #4
	movs r6, #18
	cmp r3, #1
	bls .L_08020f12
	movs r6, #16
	b .L_08020f12
.L_08020f02:
	cmp r6, #5
	beq .L_08020f0e
	cmp r6, #11
	beq .L_08020f0e
	cmp r6, #17
	bne .L_08020f12
.L_08020f0e:
	ldr r5, .L_08021064
	subs r6, #1
.L_08020f12:
	ldr r2, [r5]
	movs r3, #16
	ands r2, r3
	cmp r2, #0
	beq .L_08020f4a
	movs r0, #111
	bl Func_080f9010
	adds r6, #1
	movs r1, #1
	mov r9, r1
	cmp r6, #19
	bne .L_08020f30
	movs r6, #0
	b .L_08020f3e
.L_08020f30:
	cmp r6, #5
	beq .L_08020f3c
	cmp r6, #11
	beq .L_08020f3c
	cmp r6, #17
	bne .L_08020f3e
.L_08020f3c:
	adds r6, #1
.L_08020f3e:
	cmp r6, #18
	bne .L_08020f4a
	subs r3, r7, #4
	cmp r3, #1
	bls .L_08020f4a
	movs r6, #0
.L_08020f4a:
	ldr r3, .L_08021068
	ldr r2, [r3]
	movs r3, #8
	ands r2, r3
	cmp r2, #0
	beq .L_08020f64
	movs r0, #111
	bl Func_080f9010
	movs r2, #1
	mov r9, r2
	movs r6, #18
	movs r7, #5
.L_08020f64:
	ldr r2, .L_08021064
	ldr r5, [r2]
	movs r3, #2
	ands r5, r3
	cmp r5, #0
	beq .L_08020fa6
	movs r0, #113
	bl Func_080f9010
.L_08020f76:
	ldr r3, [sp, #28]
	cmp r3, #0
	beq .L_08020f9e
	ldr r1, [sp, #8]
	subs r3, #1
	str r3, [sp, #28]
	subs r1, #1
	movs r3, #0
	str r1, [sp, #8]
	strb r3, [r1]
	ldr r0, [sp, #40]
	bl RenderOutput_PrepareForRedraw
	ldr r0, [sp, #40]
	ldr r1, [sp, #24]
	bl UiText_DrawPaddedLabel
	movs r2, #1
	str r2, [sp, #12]
	b .L_08020d86
.L_08020f9e:
	movs r3, #1
	negs r3, r3
	str r3, [sp, #36]
	b .L_08021034
.L_08020fa6:
	ldr r3, [r2]
	movs r1, #1
	ands r3, r1
	cmp r3, #0
	bne .L_08020fb2
	b .L_08020d86
.L_08020fb2:
	movs r0, #112
	bl Func_080f9010
	cmp r6, #18
	bne .L_08020fe6
	cmp r7, #5
	bne .L_08020fde
	ldr r2, [sp, #28]
	cmp r2, #0
	bne .L_08020fc8
	b .L_08020d34
.L_08020fc8:
	ldr r2, [sp, #20]
	ldr r1, [sp, #24]
	movs r0, #0
.L_08020fce:
	ldrb r3, [r1]
	adds r0, #1
	strb r3, [r2]
	adds r1, #1
	adds r2, #1
	cmp r0, #14
	ble .L_08020fce
	b .L_08021034
.L_08020fde:
	cmp r7, #4
	beq .L_08020fe4
	b .L_08020d86
.L_08020fe4:
	b .L_08020f76
.L_08020fe6:
	mov r3, r8
	ldrh r2, [r3, #12]
	ldrh r3, [r3, #14]
	adds r3, r3, r7
	adds r2, r2, r6
	adds r3, #1
	adds r2, #1
	lsls r3, r3, #5
	adds r3, r3, r2
	ldr r1, [sp, #16]
	ldr r2, [sp, #28]
	lsls r3, r3, #1
	ldrb r3, [r3, r1]
	cmp r2, #5
	bne .L_08021006
	b .L_08020d86
.L_08021006:
	ldr r1, [sp, #8]
	adds r2, #1
	strb r3, [r1]
	adds r1, #1
	str r1, [sp, #8]
	strb r5, [r1]
	str r2, [sp, #28]
	cmp r2, #5
	bne .L_08021020
	movs r2, #1
	movs r6, #18
	movs r7, #5
	mov r9, r2
.L_08021020:
	ldr r0, [sp, #40]
	bl RenderOutput_PrepareForRedraw
	ldr r0, [sp, #40]
	ldr r1, [sp, #24]
	bl UiText_DrawPaddedLabel
	movs r3, #1
	str r3, [sp, #12]
	b .L_08020d86
.L_08021034:
	mov r0, r8
	movs r1, #2
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [sp, #40]
	bl UiWork_Finalize
	ldr r0, [sp, #44]
	bl UiWork_FinalizeEntityMatchingLocalizedId
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #36]
	add sp, #96
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_08021064:
	.4byte gKeysRepeat
.L_08021068:
	.4byte gKeyState
